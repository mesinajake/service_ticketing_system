FROM ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y \
    build-essential \
    ca-certificates \
    cmake \
    git \
    libasio-dev \
    libsqlite3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

ARG BUILD_TESTS=OFF
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=${BUILD_TESTS} \
    && cmake --build build --config Release

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    curl \
    libsqlite3-0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=build /app/build/service_ticket_backend /app/service_ticket_backend
RUN mkdir -p /app/data
COPY data/seed.sql /app/data/seed.sql

ENV APP_PORT=8080
EXPOSE 8080

CMD ["./service_ticket_backend"]
