# High-Performance Key-Value Store Engine

## Overview

A high-performance in-memory key-value store implemented in C++ with TCP networking, a custom RESP protocol, TTL-based eviction, Pub/Sub messaging, and AOF persistence.

The system is designed to support concurrent clients and demonstrates core concepts used in modern distributed and in-memory database systems.

---

## Features

### Core Storage Engine
- In-memory key-value storage using a custom hash table
- Average O(1) time complexity for GET/SET operations

### Networking
- TCP server implemented using Winsock (Windows)
- Custom implementation of RESP (Redis Serialization Protocol)

### Data Management
- TTL-based key expiration support
- Hybrid eviction strategy:
  - Lazy deletion
  - Min-heap based expiration scheduler (O(log n))

### Messaging System
- Pub/Sub model for real-time message broadcasting to subscribed clients

### Persistence
- Append-Only File (AOF) persistence
- Command log replay for recovery on startup

---

## Performance

Benchmarked under concurrent load testing:

- Throughput: ~10,000–12,000 requests per second (RPS)
- Average latency: < 5 ms
- p99 latency: ~7–10 ms
- Test configuration: 50 concurrent clients, 100,000 operations

---

## Build Instructions

### Requirements
- C++17 compatible compiler
- CMake (>= 3.10)
- Windows (uses Winsock `ws2_32`)

### Build Steps

```bash
mkdir build
cd build
cmake ..
cmake --build .

## Run
./KeyValueStoreEngine

## Project Structure
src/
├── server/        # TCP server implementation
├── parser/        # RESP command parsing
├── protocol/      # RESP serialization
├── storage/       # Core storage + persistence
├── commands/      # Command implementations
├── main.cpp

## Design Goals
High throughput under concurrent load
Low-latency in-memory operations
Modular command-based architecture
Redis-inspired protocol design (custom implementation)
## Notes

This project focuses on systems programming concepts such as:

Event-driven server design
Memory-efficient data structures
Concurrency handling
Persistence mechanisms

## License

This project is intended for educational and portfolio purposes.
