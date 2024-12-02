- [Architecture](#architecture)
  - [Software Architecture Documentation](#software-architecture-documentation)
    - [**1. Overview**](#1-overview)
  - [**2. System Components**](#2-system-components)
  - [**3. Component Interaction Diagram**](#3-component-interaction-diagram)
  - [**4. Component Details**](#4-component-details)
    - [**4.1 ProductCache**](#41-productcache)
    - [**4.2 ProductService**](#42-productservice)
    - [**4.3 FakeDatabase**](#43-fakedatabase)
    - [**4.4 Logger**](#44-logger)
  - [**5. Thread Safety and Concurrency**](#5-thread-safety-and-concurrency)

## Architecture
### Software Architecture Documentation

#### **1. Overview**
This software implements a caching mechanism for product data in an e-commerce system. It optimizes performance by caching frequently accessed product details using an LRU (Least Recently Used) cache. It supports concurrent access from multiple clients and includes thread safety mechanisms to ensure data consistency.

---

### **2. System Components**
The architecture consists of several core components, each with a well-defined role:

1. **ProductCache**:  
   An in-memory cache that stores product details using an LRU eviction policy.
   
2. **ProductService**:  
   Acts as a mediator between the cache and the database. It routes requests to either the cache or the database, ensuring efficient data retrieval.

3. **FakeDatabase**:  
   A mock database that simulates the retrieval of product data. It provides thread-safe methods for fetching product details and product counts.

4. **Logger**:  
   Handles logging for the entire system, including different log levels such as INFO and WARNING. Logs are written to a file.

5. **Tests (ProductCacheTest, ProductServiceTest, FakeDatabaseTest)**:  
   Unit tests ensure the correctness of the caching logic, database access, and thread safety. Implemented using Google Test (GTest) and Google Mock (GMock).

---

### **3. Component Interaction Diagram**

```plaintext
┌────────────────┐       ┌────────────────┐       ┌────────────────┐
│ Client Threads │──────▶│ ProductService │──────▶│  FakeDatabase  │
└────────────────┘       └────────────────┘       └────────────────┘
        │                          │
        │                          ▼
        │                  ┌────────────────┐
        └─────────────────▶│  ProductCache  │
                           └────────────────┘
```

- **Clients** request product details in parallel.
- **ProductService** checks the cache first and queries the database only if necessary.
- **ProductCache** implements LRU eviction.
- **FakeDatabase** is queried when the cache misses.
- **Logger** logs all significant events and actions.

---

### **4. Component Details**

#### **4.1 ProductCache**
**Responsibilities:**
- Store product details with a maximum capacity using an LRU policy.
- Evict the least recently used item when full.
- 
---

#### **4.2 ProductService**
**Responsibilities:**
- Interface between the client, cache, and database.
- Retrieve product details from the cache or database.
- Populate the cache with database results when cache misses occur.

---

#### **4.3 FakeDatabase**
**Responsibilities:**
- Simulate database operations with hardcoded product data.
- Provide thread-safe access to product data.

---

#### **4.4 Logger**
**Responsibilities:**
- Centralized logging for the entire system.
- Supports different log levels (`INFO`, `WARNING`, etc.).

---

### **5. Thread Safety and Concurrency**
The system is designed to handle concurrent access by multiple threads:

1. **Thread Safety in Cache:**
   - `std::shared_mutex` ensures that multiple threads can read simultaneously but only one can write at a time.
   - `std::shared_lock` for reads and `std::unique_lock` for writes.

2. **Thread Management:**
   - `std::jthread` ensures safe thread lifecycle management with automatic joining.

3. **Race Condition Avoidance:**
   - Proper locking mechanisms are in place to prevent data races during cache insertion and eviction.