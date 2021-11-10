FROM ubuntu:latest

RUN apt-get update
RUN apt-get -y install git wget python3-pip jq
RUN wget https://github.com/libbitcoin/libbitcoin-explorer/releases/download/v3.2.0/bx-linux-x64-qrcode 
RUN chmod +x bx-linux-x64-qrcode
RUN cp bx-linux-x64-qrcode /usr/local/bin/bx

WORKDIR /home

COPY requirements.txt requirements.txt

RUN pip3 install -r requirements.txt
RUN python3 -m bash_kernel.install

WORKDIR /home/exercises

CMD ["jupyter", "notebook", "--ip", "0.0.0.0", "--allow-root"]