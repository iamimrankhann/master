FROM gcc:latest
WORKDIR /app
RUN apt-get update && \
    apt-get install -y \
    cmake \
    && rm -rf /var/lib/apt/lists/*
COPY . /app
COPY LAMP.csv /app/
COPY config.txt /app/
RUN mkdir build && cd build && \
    cmake .. && \
    make
CMD ["/app/build/lamp_life_calculator"]
