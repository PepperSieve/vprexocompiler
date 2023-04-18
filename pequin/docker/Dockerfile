FROM ubuntu:bionic

ENV PEQUIN /opt/pequin
RUN set -ex \
    && buildDeps=" \
    bash python2.7 libpython2.7 libpython2.7-dev openjdk-8-jdk \
    golang build-essential gcc-5 g++-5 gcc-multilib g++-multilib ant  \
    ant-optional make time libboost-all-dev libgmp10 libgmp-dev   \
    zlib1g zlib1g-dev libssl-dev cmake git pkg-config \
    " \
    && apt-get update \
    && apt-get install -y --no-install-recommends $buildDeps \
    && rm -rf /var/lib/apt/lists/*

RUN rm /usr/bin/gcc \
    && rm /usr/bin/g++ \
    && ln -s /usr/bin/gcc-5 /usr/bin/gcc \
    && ln -s /usr/bin/g++-5 /usr/bin/g++

COPY . $PEQUIN
RUN cd $PEQUIN/thirdparty && ./install_pepper_deps.sh \
    && cd $PEQUIN && ./install_buffet.sh \
    && mv $PEQUIN/thirdparty/libsnark /tmp && rm -rf $PEQUIN/thirdparty/* \
    && mv /tmp/libsnark $PEQUIN/thirdparty/ \
    && rm -rf $PEQUIN/compiler/buffetfsm/llvm $PEQUIN/compiler/buffetfsm/llvm-build
