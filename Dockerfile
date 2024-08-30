# FROM gcc:latest
# WORKDIR /app
# RUN apt-get update && \
#     apt-get install -y \
#     cmake \
#     && rm -rf /var/lib/apt/lists/*
# COPY . /app
# COPY LAMP.csv /app/
# COPY config.txt /app/
# RUN mkdir build && cd build && \
#     cmake .. && \
#     make
# CMD ["/app/build/lamp_life_calculator"]
# Use the gcc image as the base image for building
# Use Alpine as the base image for building
FROM alpine:latest AS builder
WORKDIR /app

# Install necessary build tools and dependencies
RUN apt-get update && \
    apt-get install -y cmake && \
    rm -rf /var/lib/apt/lists/*

# Copy the application files
COPY . /app
COPY LAMP.csv /app/
COPY config.txt /app/

# Build the application
RUN mkdir build && cd build && \
    cmake .. && \
    make
FROM alpine:latest
# Install runtime libraries
RUN apk add --no-cache libstdc++ libgcc
WORKDIR /app

# Copy the built application and necessary files from the builder stage
COPY --from=builder /app/build/lamp_life_calculator /app/lamp_life_calculator
COPY --from=builder /app/LAMP.csv /app/
COPY --from=builder /app/config.txt /app/

# Set the command to run the application
CMD ["/app/lamp_life_calculator"]



