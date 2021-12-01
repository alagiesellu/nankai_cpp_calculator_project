FROM gcc:11.1.0

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y cmake libgtest-dev libboost-test-dev && rm -rf /var/lib/apt/lists/*

WORKDIR /
COPY . .

RUN chmod +x run.sh
ENTRYPOINT ["/run.sh"]
