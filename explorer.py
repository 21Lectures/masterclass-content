import sys
from dataclasses import dataclass

import requests
import json

def query_api_endpoints(
    urls: list[str],
) -> requests.Response:
    for url in urls:
        try:
            response = requests.get(url, timeout=10)
        except requests.ReadTimeout:
            continue
        if response.status_code == 200:
            return response
    raise Exception("No Bitcoin block explorer returned 200 response.")


@dataclass
class Utxo:
    txid: str
    vout: int
    value: int



def broadcast_tx(tx: str) -> str:
    for url in esplora_api_endpoints:
        try:
            if (
                response := requests.post(f"{url}/tx", tx, timeout=10)
            ).status_code == 200:
                return response.text
        except requests.ReadTimeout:
            continue
    raise Exception("No returned 200 response from a Bitcoin broadcast API.")

def get_unspents(
    address: str,
) -> str:
    api_endpoint_urls = [
        f"{api_endpoint}/address/{address}/utxo"
        for api_endpoint in esplora_api_endpoints
    ]
    return json.dumps(query_api_endpoints(api_endpoint_urls).json(),indent=4)

def get_transaction(
    txid: str,
) -> str:
    api_endpoint_urls = [
        f"{api_endpoint}/tx/{txid}"
        for api_endpoint in esplora_api_endpoints
    ]
    return json.dumps(query_api_endpoints(api_endpoint_urls).json(),indent=4)

def get_address_history(
    address: str,
) -> str:
    api_endpoint_urls = [
        f"{api_endpoint}/address/{address}/txs"
        for api_endpoint in esplora_api_endpoints
    ]
    return json.dumps(query_api_endpoints(api_endpoint_urls).json(),indent=4)

def get_chain_height() -> int:
    api_endpoint_urls = [
        f"{api_endpoint}/blocks/tip/height"
        for api_endpoint in esplora_api_endpoints
    ]
    return query_api_endpoints(api_endpoint_urls).json()


esplora_api_endpoints = [
    "https://blockstream.info/testnet/api",
    "https://mempool.space/testnet/api",
]

broadcast_endpoint_url = "https://blockstream.info/testnet/tx/push"

def main():
    if len(sys.argv) < 2:
        print("Usage: explorer <command>")
        print("Available commands are: 'utxos', 'transaction', 'height', 'broadcast', 'history'")
        return
    match sys.argv[1]:
        case "utxos":
            if len(sys.argv) != 3:
                print("usage: python explorer.py utxos <address>")
                return
            print(f"Unspents of address {sys.argv[2]}: {get_unspents(sys.argv[2])}")
        case "history":
            if len(sys.argv) != 3:
                print("usage: python explorer.py history <address>")
                return
            print(f"History of address {sys.argv[2]}: {get_address_history(sys.argv[2])}")
        case "transaction":
            if len(sys.argv) != 3:
                print("usage: python explorer.py transaction <txid>")
                return
            print(get_transaction(sys.argv[2]))
        case "height":
            print(f"Chain height: {get_chain_height()}")
        case "broadcast":
            if len(sys.argv) != 3:
                print("usage: python explorer.py broadcast <tx-serialization>")
                return
            print(f"https://blockstream.info/testnet/tx/{broadcast_tx(sys.argv[2])}")
        case _:
            print("Available commands are: 'utxos', 'transaction', 'height', 'broadcast', 'history'")

if __name__ == "__main__":
    main()