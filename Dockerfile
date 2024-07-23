# Use a multi-stage build
FROM gcc:latest AS builder

WORKDIR /app

# Install only necessary packages
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Copy only necessary files
COPY CMakeLists.txt /app/
COPY src /app/src

# Build the application
RUN mkdir build && cd build && \
    cmake .. && \
    make

# Start a new stage with a minimal base image
FROM debian:slim

WORKDIR /app

# Copy only the built executable from the previous stage
COPY --from=builder /app/build/lamp_life_calculator /app/
COPY LAMP.csv /app/
COPY config.txt /app/

CMD ["/app/lamp_life_calculator"]
