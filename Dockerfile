FROM --platform=linux/amd64 ubuntu:latest

RUN apt-get update
RUN apt-get -y install git wget python3-pip jq python3-venv
RUN wget https://github.com/21lectures/bx-binary/raw/master/bx
RUN chmod +x bx
RUN cp bx /usr/local/bin/bx

WORKDIR /home
COPY requirements.txt requirements.txt
COPY explorer.py explorer.py

RUN python3 -m venv /app/venv
ENV PATH="/app/venv/bin:$PATH"
COPY requirements.txt .

RUN pip3 install --no-cache-dir -r requirements.txt
RUN python3 -m bash_kernel.install

WORKDIR /home/exercises

CMD ["jupyter", "notebook", "--ip", "0.0.0.0", "--allow-root"]