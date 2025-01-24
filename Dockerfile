#Final Optimized image
FROM alpine:latest AS builder

WORKDIR /lamp


# Install necessary build tools and dependencies
RUN apk add --no-cache \
    build-base \
    cmake

# Copy the application files
COPY . /lamp

# Build the application with static linking
RUN mkdir build && cd build && \
    cmake -DCMAKE_EXE_LINKER_FLAGS="-static" .. && \
    make

# Start a new stage with a minimal Alpine image
FROM alpine:latest

# Install runtime libraries
RUN apk add --no-cache libstdc++ libgcc

# Create the AGGREGATION directory in the final image
WORKDIR /lamp

# Copy the built application from the builder stage
COPY --from=builder /lamp/build/lamp_life_calculator /app/lamp_life_calculator

# Copy config.txt and data directory
COPY LAMP.csv /lamp/
COPY config.txt /lamp/

# Set the command to run the application
CMD ["/app/lamp_life_calculator"]

