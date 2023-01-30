FROM ubuntu:bionic

ENV PEQUIN /opt/pequin

RUN set -ex \
    && buildDeps=" \
    bash python2.7 libpython2.7 libpython2.7-dev openjdk-11-jdk \
    golang build-essential gcc-5 g++-5 gcc-multilib g++-multilib ant  \
    ant-optional make time libboost-all-dev libgmp10 libgmp-dev   \
    zlib1g zlib1g-dev libssl-dev cmake git pkg-config \
    " \
    && apt-get update \
    && apt-get install apt-transport-https curl gnupg -yqq \
    && echo "deb https://repo.scala-sbt.org/scalasbt/debian all main" | tee /etc/apt/sources.list.d/sbt.list \
    && echo "deb https://repo.scala-sbt.org/scalasbt/debian /" | tee /etc/apt/sources.list.d/sbt_old.list \
    && curl -sL "https://keyserver.ubuntu.com/pks/lookup?op=get&search=0x2EE0EA64E40A89B84B2DF73499E82A75642AC823" | gpg --no-default-keyring --keyring gnupg-ring:/etc/apt/trusted.gpg.d/scalasbt-release.gpg --import \
    && chmod 644 /etc/apt/trusted.gpg.d/scalasbt-release.gpg \
    && apt-get update \
    && apt-get install -y --no-install-recommends sbt \
    && apt-get install -y --no-install-recommends $buildDeps \
    && apt-get install sbt \
    && rm -rf /var/lib/apt/lists/*

RUN rm /usr/bin/gcc \
    && rm /usr/bin/g++ \
    && ln -s /usr/bin/gcc-5 /usr/bin/gcc \
    && ln -s /usr/bin/g++-5 /usr/bin/g++

COPY . $VPREXOCOMPILER
RUN cd $VPREXOCOMPILER/pequin/thirdparty && ./install_pepper_deps.sh \
    && cd $VPREXOCOMPILER/pequin && ./install_buffet.sh \
    && mv $VPREXOCOMPILER/pequin/thirdparty/libsnark /tmp && rm -rf $VPREXOCOMPILER/pequin/thirdparty/* \
    && mv /tmp/libsnark $VPREXOCOMPILER/pequin/thirdparty/ \
    && rm -rf $VPREXOCOMPILER/pequin/compiler/buffetfsm/llvm $VPREXOCOMPILER/pequin/compiler/buffetfsm/llvm-build

RUN bash exec.sh find_min