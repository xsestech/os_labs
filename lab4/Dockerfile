FROM ubuntu:latest
LABEL authors="xsestech"

RUN apt-get update && apt-get install -y \
    clang cmake build-essential ninja-build gdb strace

ARG UID=1000
RUN useradd -m -u ${UID} -s /bin/bash builder
USER builder



WORKDIR app
COPY . .

ENTRYPOINT ["/bin/bash"]