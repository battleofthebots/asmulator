FROM ghcr.io/battleofthebots/botb-base-image:latest
ARG NAME=asmrepel
LABEL org.opencontainers.image.title=$NAME org.opencontainers.image.description=$NAME org.opencontainers.image.url=https://github.com/battleofthebots/$NAME org.opencontainers.image.source=https://github.com/battleofthebots/$NAME org.opencontainers.image.version=main
WORKDIR /opt/
COPY . .
RUN apt-get update -y && \
    apt install -y git cmake pkg-config curl && \
    git clone https://github.com/unicorn-engine/unicorn.git && \
    cd unicorn; mkdir build; cd build; cmake ../ -DCMAKE_BUILD_TYPE=Release; make install 
RUN ldconfig && make;
ENTRYPOINT ["/opt/bin/asmulator"]
