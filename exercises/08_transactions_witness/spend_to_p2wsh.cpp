#include <bitcoin/bitcoin.hpp>

int main()
{
    //**************************************************************
    // 1. Restore wallet and generate spending / receiving key pairs
    //**************************************************************

    std::string my_sentence = "amount right cheese defy click eight slight strategy replace earn simple labor";
    const auto my_word_list = bc::split(my_sentence, " ", true);

    const auto hd_seed = bc::wallet::decode_mnemonic(my_word_list);

    bc::wallet::hd_private hd_master_private(bc::to_chunk(hd_seed),
        bc::wallet::hd_private::testnet);
    const auto hd_master_public = hd_master_private.to_public();

    // We will be spending a p2pkh output script controlled by
    // the following key: m/44'/1'/1'/0/9
    //-------------------------------------------------------------

    const auto hd_m_44h_1h_1h_0_9 = hd_master_private
        .derive_private(44 + bc::wallet::hd_first_hardened_key)
        .derive_private(1 + bc::wallet::hd_first_hardened_key)
        .derive_private(1 + bc::wallet::hd_first_hardened_key)
        .derive_private(0)
        .derive_private(9);
    const auto secret_44h_1h_1h_0_9 = hd_m_44h_1h_1h_0_9.secret();
    const auto pubkey_44h_1h_1h_0_9 =
        hd_m_44h_1h_1h_0_9.to_public().point();

    bc::wallet::payment_address payment_address_44h_1h_1h_0_9(
        pubkey_44h_1h_1h_0_9, bc::wallet::payment_address::testnet_p2kh);

    // Generate receiving keys for the P2SH(Multisig) output.
    //-------------------------------------------------------------

    // Three Receiving public keys in 3rd wallet account.

    const auto hd_m_44h_1h_2h_0 = hd_master_private
        .derive_private(44 + bc::wallet::hd_first_hardened_key)
        .derive_private(1 + bc::wallet::hd_first_hardened_key)
        .derive_private(2 + bc::wallet::hd_first_hardened_key)
        .derive_private(0);

    const auto hd_m_44h_1h_2h_0_9 = hd_m_44h_1h_2h_0.derive_private(9);
    const auto pubkey_44h_1h_2h_0_9 = hd_m_44h_1h_2h_0_9.to_public().point();

    const auto hd_m_44h_1h_2h_0_10 = hd_m_44h_1h_2h_0.derive_private(10);
    const auto pubkey_44h_1h_2h_0_10 = hd_m_44h_1h_2h_0_10.to_public().point();

    const auto hd_m_44h_1h_2h_0_11 = hd_m_44h_1h_2h_0.derive_private(11);
    const auto pubkey_44h_1h_2h_0_11 = hd_m_44h_1h_2h_0_11.to_public().point();

    //**************************************************************
    // 2. Fetch previous P2PKH UTXO which we will spend.
    //**************************************************************

    // Previous TX hash.
    const auto prev_tx_literal =
        "86ad635645920497c233cc33556463fa6258ab8f9c5f7d12748a6638216e3363";
    bc::hash_digest prev_tx_hash;
    bc::decode_hash(prev_tx_hash, prev_tx_literal);

    // Previous UXTO prev_index.
    uint32_t prev_index = 9;
    bc::chain::output_point uxto_to_spend(prev_tx_hash, prev_index);

    // Previous script.
    const auto prev_p2pkh_output_script =
        bc::chain::script::to_pay_key_hash_pattern(
            bc::bitcoin_short_hash(pubkey_44h_1h_1h_0_9));

    // Previous output amount.
    uint64_t previous_output_amount(499543);

    //**************************************************************
    // 3. Build, Sign & Complete Transaction.
    //**************************************************************

    // A. Encode P2SH(multisig) output.
    //-------------------------------------------------------------

    // Witness script: 2/3 Multisig.
    uint8_t signature_number(2u);
    bc::point_list points;
    points.push_back(pubkey_44h_1h_2h_0_9);
    points.push_back(pubkey_44h_1h_2h_0_10);
    points.push_back(pubkey_44h_1h_2h_0_11);
    bc::chain::script witness_script =
        bc::chain::script::to_pay_multisig_pattern(signature_number, points);

    // P2WSH output script.
    //    0 [32-byte sha256(witness script)]
    const auto witness_script_hash =
        bc::sha256_hash_chunk(witness_script.to_data(false)); // no prefix.
    bc::machine::operation::list p2sh_operations;
    p2sh_operations.push_back(
        bc::machine::operation(bc::machine::opcode::push_size_0));
    p2sh_operations.push_back(
        bc::machine::operation(witness_script_hash));

    // Compute fees.
    // tx bytes: 4 + 1 + 1*(32+4+110+4) + 1 + 1*(8+1+1+34) + 4 = 202
    // 1 non-witness sigop: 1 * 400
    // 499543 - 1 * 400 - 202 = 498941
    uint64_t output_amount(498941);

    // Build output.
    bc::chain::output p2wsh_output(output_amount, p2sh_operations);

    // B. Build Transaction for signing.
    //-------------------------------------------------------------

    // Build input0 object.
    bc::chain::input input0;
    input0.set_previous_output(uxto_to_spend);
    input0.set_sequence(bc::max_input_sequence);

    // Build Transaction.
    bc::chain::transaction tx;
    tx.set_version(1u);
    tx.inputs().push_back(input0);
    tx.outputs().push_back(p2wsh_output);

    // Signature.
    bc::endorsement signature;
    uint8_t input0_index(0u);
    bc::chain::script::create_endorsement(signature, secret_44h_1h_1h_0_9,
      prev_p2pkh_output_script, tx, input0_index,
      bc::machine::sighash_algorithm::all);

    // B. Finalise Transaction.
    //-------------------------------------------------------------

    // Set input script into transaction.
    bc::machine::operation::list p2pkh_operations;
    p2pkh_operations.push_back(bc::machine::operation(signature));
    p2pkh_operations.push_back(bc::machine::operation(
        bc::to_chunk(pubkey_44h_1h_1h_0_9)));
    bc::chain::script p2pkh_input_script(p2pkh_operations);
    tx.inputs()[0].set_script(p2pkh_input_script);

    std::cout << bc::encode_base16(tx.to_data()) << std::endl;
}
// g++ -std=c++11 -o spend_to_p2wsh spend_to_p2wsh.cpp $(pkg-config --cflags libbitcoin --libs libbitcoin)
