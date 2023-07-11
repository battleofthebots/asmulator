FROM ghcr.io/battleofthebots/botb-base-image:ubuntu-20.04-defcon-2023
WORKDIR /opt/
COPY . .
RUN apt-get update -y && \
    apt install -y git cmake pkg-config curl && \
    git clone https://github.com/unicorn-engine/unicorn.git && \
    cd unicorn; mkdir build; cd build; cmake ../ -DCMAKE_BUILD_TYPE=Release; make install;

ENTRYPOINT ["/bin/bash"]
