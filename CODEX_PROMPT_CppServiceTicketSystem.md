# Codex Prompt — C++ Service Ticket Management System
## Interview-Ready Full-Stack Project | Associate Software Engineer Portfolio

---

## PROMPT HEADER (Paste this at the very top when sending to Claude Code / Codex)

```
Act as a senior full-stack software engineer building a portfolio project for an Associate Software Engineer interview.
Use C++17 with CMake, the Crow C++ web framework, and SQLite3 for the backend.
Use React + TypeScript + Tailwind CSS + Vite for the frontend.
Structure the backend with clearly separated Database, Model, Service, Controller, and Utils layers.
Write clean, readable C++17 code suitable for a junior-level portfolio.
Add inline comments only where logic is non-obvious.
Ensure all API endpoints return proper HTTP status codes and JSON responses.
Automatically create activity log entries on every important action.
Keep all business logic inside Service classes — never directly in main.cpp or Controllers.
Build everything in this exact order:
  1. Database layer + schema + seed data
  2. Models
  3. Services
  4. Controllers
  5. main.cpp route registration
  6. Frontend pages and components
  7. Docker configuration
  8. README.md
```

---

## PROJECT OVERVIEW

**Project Name:** C++ Service Ticket Management System for IT Support Operations

**Purpose:** Simulate how IT support teams manage service tickets, client issues, bug reports, and task assignments. This is an interview-ready full-stack project that demonstrates C++, OOP, REST API design, database integration, clean architecture, and real-world business logic.

**Target Role:** Associate Software Engineer

---

## TECH STACK

### Backend
- **Language:** C++17
- **Framework:** Crow C++ web framework (header-only, easy to set up)
- **Build System:** CMake 3.15+
- **Database:** SQLite3 (via sqlite3.h C API)
- **JSON:** nlohmann/json (header-only)
- **Testing:** Catch2 v3 (unit tests for Service layer)
- **Architecture:** OOP with layered structure (Controller → Service → Database)

### Frontend
- **Framework:** React 18 + TypeScript
- **Build Tool:** Vite
- **Styling:** Tailwind CSS v3
- **HTTP Client:** Axios
- **Routing:** React Router v6
- **UI Components:** Custom Tailwind components (no heavy UI libraries needed)

### DevOps / Tools
- **Containerization:** Docker + Docker Compose
- **API Testing:** Postman (collection file included in repository)
- **Version Control:** Git + GitHub
- **IDE:** Visual Studio Code

---

## FEATURES

### 1. Dashboard
Display a summary page with the following metrics:
- Total tickets count
- Open tickets count
- In Progress tickets count
- Resolved tickets count
- Closed tickets count
- Critical priority tickets count
- List of the 5 most recently created tickets

### 2. Ticket Management
Full CRUD operations for service tickets including:
- Create a new ticket
- View all tickets (with search and filter)
- View ticket details
- Edit ticket information
- Delete a ticket
- Update ticket status
- Assign ticket to an engineer
- Search tickets by: ID, title, requester name, or assigned engineer name
- Filter tickets by: status, priority, category, and date range

**Ticket Fields:**
| Field | Type | Notes |
|---|---|---|
| id | INTEGER | Auto-incremented primary key |
| title | TEXT | Required |
| description | TEXT | Required |
| requester_name | TEXT | Required |
| requester_email | TEXT | Required, valid email format |
| category | TEXT | See allowed values below |
| priority | TEXT | See allowed values below |
| status | TEXT | See allowed values below |
| assigned_engineer_id | INTEGER | Nullable FK to engineers table |
| resolution_notes | TEXT | Nullable, filled when resolving |
| created_at | DATETIME | Auto set on insert |
| updated_at | DATETIME | Auto updated on every change |

**Priority values:** `Low`, `Medium`, `High`, `Critical`

**Status values:** `Open`, `In Progress`, `Resolved`, `Closed`

**Category values:** `Software Issue`, `Hardware Issue`, `Network Issue`, `Account Access`, `System Bug`, `Client Request`, `Others`

### 3. Engineer Management
A simple module to manage support engineers:
- Add a new engineer
- View all engineers
- View single engineer details
- Assign tickets to engineers
- Display count of active assigned tickets per engineer

**Engineer Fields:**
| Field | Type | Notes |
|---|---|---|
| id | INTEGER | Auto-incremented primary key |
| full_name | TEXT | Required |
| email | TEXT | Required, unique |
| role | TEXT | Required (e.g. Junior Engineer, Senior Engineer) |
| department | TEXT | Required (e.g. IT Support, Infrastructure) |
| created_at | DATETIME | Auto set on insert |

### 4. Activity Logs
Every important action must automatically create a log entry. The system must log:
- Ticket created
- Ticket updated
- Ticket assigned to engineer
- Ticket status changed
- Ticket resolved
- Ticket closed
- Ticket deleted

**Log Fields:**
| Field | Type | Notes |
|---|---|---|
| id | INTEGER | Auto-incremented primary key |
| ticket_id | INTEGER | FK to tickets table |
| action | TEXT | Short action name (e.g. TICKET_CREATED) |
| description | TEXT | Human-readable description |
| created_at | DATETIME | Auto set on insert |

### 5. Reports
A reports page showing visual summaries:
- Tickets grouped by status
- Tickets grouped by priority
- Tickets grouped by category
- Ticket count assigned per engineer

### 6. Input Validation
All API endpoints must validate:
- Required fields are not empty
- Email fields follow a valid email format
- Enum fields (priority, status, category) only accept defined values
- IDs in URL parameters are valid positive integers
- Return HTTP `400 Bad Request` with a descriptive JSON error message on validation failure

---

## API ENDPOINTS

### Tickets
```
GET    /api/tickets                    — Get all tickets (supports ?search=, ?status=, ?priority=, ?category= query params)
GET    /api/tickets/:id                — Get single ticket by ID
POST   /api/tickets                    — Create new ticket
PUT    /api/tickets/:id                — Update full ticket
DELETE /api/tickets/:id                — Delete ticket
PATCH  /api/tickets/:id/status         — Update status only { "status": "In Progress" }
PATCH  /api/tickets/:id/assign         — Assign engineer { "engineer_id": 2 }
```

### Engineers
```
GET    /api/engineers                  — Get all engineers with their assigned ticket count
GET    /api/engineers/:id              — Get single engineer
POST   /api/engineers                  — Create new engineer
```

### Activity Logs
```
GET    /api/logs                       — Get all activity logs (most recent first)
GET    /api/tickets/:id/logs           — Get all logs for a specific ticket
```

### Reports
```
GET    /api/reports/summary            — Dashboard summary counts
GET    /api/reports/status             — Ticket counts grouped by status
GET    /api/reports/priority           — Ticket counts grouped by priority
GET    /api/reports/category           — Ticket counts grouped by category
GET    /api/reports/engineers          — Ticket counts per engineer
```

### Health Check
```
GET    /api/health                     — Returns { "status": "ok" } for Docker health checks
```

---

## DATABASE SCHEMA

### Table: tickets
```sql
CREATE TABLE IF NOT EXISTS tickets (
    id                  INTEGER PRIMARY KEY AUTOINCREMENT,
    title               TEXT    NOT NULL,
    description         TEXT    NOT NULL,
    requester_name      TEXT    NOT NULL,
    requester_email     TEXT    NOT NULL,
    category            TEXT    NOT NULL,
    priority            TEXT    NOT NULL,
    status              TEXT    NOT NULL DEFAULT 'Open',
    assigned_engineer_id INTEGER NULL REFERENCES engineers(id) ON DELETE SET NULL,
    resolution_notes    TEXT    NULL,
    created_at          DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at          DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

### Table: engineers
```sql
CREATE TABLE IF NOT EXISTS engineers (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    full_name   TEXT    NOT NULL,
    email       TEXT    NOT NULL UNIQUE,
    role        TEXT    NOT NULL,
    department  TEXT    NOT NULL,
    created_at  DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

### Table: activity_logs
```sql
CREATE TABLE IF NOT EXISTS activity_logs (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    ticket_id   INTEGER NOT NULL REFERENCES tickets(id) ON DELETE CASCADE,
    action      TEXT    NOT NULL,
    description TEXT    NOT NULL,
    created_at  DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

### Seed Data File: `backend/data/seed.sql`
Create a seed file with:
- At least 3 engineers across different departments
- At least 10 tickets with varying status, priority, and category values
- At least 5 activity log entries

This seed file should be runnable via:
```bash
sqlite3 backend/data/tickets.db < backend/data/seed.sql
```

---

## FOLDER STRUCTURE

```
service-ticket-system/
│
├── backend/
│   ├── src/
│   │   ├── main.cpp                        ← App entry, route registration, CORS config
│   │   │
│   │   ├── config/
│   │   │   └── Config.h                    ← Port, DB path, app name constants
│   │   │
│   │   ├── database/
│   │   │   ├── Database.cpp                ← SQLite connection, query execution
│   │   │   └── Database.h
│   │   │
│   │   ├── models/
│   │   │   ├── Ticket.cpp                  ← Ticket struct + JSON serialization
│   │   │   ├── Ticket.h
│   │   │   ├── Engineer.cpp                ← Engineer struct + JSON serialization
│   │   │   ├── Engineer.h
│   │   │   ├── ActivityLog.cpp             ← ActivityLog struct + JSON serialization
│   │   │   └── ActivityLog.h
│   │   │
│   │   ├── controllers/
│   │   │   ├── TicketController.cpp        ← Handles HTTP for /api/tickets
│   │   │   ├── TicketController.h
│   │   │   ├── EngineerController.cpp      ← Handles HTTP for /api/engineers
│   │   │   ├── EngineerController.h
│   │   │   ├── LogController.cpp           ← Handles HTTP for /api/logs
│   │   │   ├── LogController.h
│   │   │   ├── ReportController.cpp        ← Handles HTTP for /api/reports
│   │   │   └── ReportController.h
│   │   │
│   │   ├── services/
│   │   │   ├── TicketService.cpp           ← All ticket business logic
│   │   │   ├── TicketService.h
│   │   │   ├── EngineerService.cpp         ← All engineer business logic
│   │   │   ├── EngineerService.h
│   │   │   ├── LogService.cpp              ← Activity log creation logic
│   │   │   ├── LogService.h
│   │   │   ├── ReportService.cpp           ← Report query aggregations
│   │   │   └── ReportService.h
│   │   │
│   │   └── utils/
│   │       ├── Validator.cpp               ← Input validation functions
│   │       ├── Validator.h
│   │       ├── ErrorCodes.h                ← Error message string constants
│   │       └── JsonHelper.h                ← Shared JSON response helper functions
│   │
│   ├── tests/
│   │   ├── CMakeLists.txt                  ← Catch2 test build config
│   │   ├── test_ticket_service.cpp         ← Unit tests for TicketService
│   │   ├── test_engineer_service.cpp       ← Unit tests for EngineerService
│   │   └── test_validator.cpp              ← Unit tests for Validator
│   │
│   ├── data/
│   │   ├── tickets.db                      ← SQLite database file (git-ignored)
│   │   └── seed.sql                        ← Sample data for development/demo
│   │
│   ├── CMakeLists.txt                      ← Main backend build file
│   └── Dockerfile
│
├── frontend/
│   ├── src/
│   │   ├── api/
│   │   │   ├── ticketApi.ts                ← All ticket API calls
│   │   │   ├── engineerApi.ts              ← All engineer API calls
│   │   │   ├── logApi.ts                   ← All log API calls
│   │   │   ├── reportApi.ts                ← All report API calls
│   │   │   └── axiosInstance.ts            ← Axios base config (base URL, headers)
│   │   │
│   │   ├── components/
│   │   │   ├── Navbar.tsx                  ← Top navigation bar
│   │   │   ├── Sidebar.tsx                 ← Side navigation links
│   │   │   ├── TicketCard.tsx              ← Ticket summary card component
│   │   │   ├── TicketTable.tsx             ← Ticket list table with sort/filter
│   │   │   ├── StatusBadge.tsx             ← Colored badge for ticket status
│   │   │   ├── PriorityBadge.tsx           ← Colored badge for ticket priority
│   │   │   ├── LoadingSpinner.tsx          ← Reusable loading indicator
│   │   │   └── ErrorMessage.tsx            ← Reusable error display component
│   │   │
│   │   ├── pages/
│   │   │   ├── Dashboard.tsx               ← Summary metrics + recent tickets
│   │   │   ├── Tickets.tsx                 ← Full ticket list with search + filter
│   │   │   ├── CreateTicket.tsx            ← New ticket form
│   │   │   ├── TicketDetails.tsx           ← Full ticket view + logs timeline
│   │   │   ├── EditTicket.tsx              ← Edit ticket form
│   │   │   ├── Engineers.tsx               ← Engineer list + add engineer form
│   │   │   └── Reports.tsx                 ← Charts and grouped summaries
│   │   │
│   │   ├── types/
│   │   │   └── index.ts                    ← TypeScript interfaces for all models
│   │   │
│   │   ├── App.tsx                         ← Route definitions
│   │   └── main.tsx                        ← React entry point
│   │
│   ├── package.json
│   ├── vite.config.ts
│   ├── tailwind.config.js
│   ├── tsconfig.json
│   └── Dockerfile
│
├── postman/
│   └── ServiceTicketSystem.postman_collection.json  ← All API requests pre-built
│
├── docker-compose.yml
├── .gitignore                              ← Ignore: *.db, /build, node_modules, .env
├── .env.example                            ← Example environment variables
└── README.md
```

---

## BACKEND IMPLEMENTATION REQUIREMENTS

### Config.h
Create a config header with constants:
```cpp
// src/config/Config.h
#pragma once
#include <string>

namespace Config {
    const int    PORT        = 8080;
    const std::string DB_PATH     = "./data/tickets.db";
    const std::string APP_NAME    = "Service Ticket System";
    const std::string API_VERSION = "v1";
}
```

### ErrorCodes.h
Create string constants for all validation/error messages:
```cpp
// src/utils/ErrorCodes.h
#pragma once
#include <string>

namespace ErrorCodes {
    const std::string FIELD_REQUIRED       = "Field is required and cannot be empty.";
    const std::string INVALID_EMAIL        = "Invalid email format.";
    const std::string INVALID_PRIORITY     = "Priority must be: Low, Medium, High, or Critical.";
    const std::string INVALID_STATUS       = "Status must be: Open, In Progress, Resolved, or Closed.";
    const std::string INVALID_CATEGORY     = "Category must be one of the defined values.";
    const std::string TICKET_NOT_FOUND     = "Ticket not found.";
    const std::string ENGINEER_NOT_FOUND   = "Engineer not found.";
    const std::string EMAIL_ALREADY_EXISTS = "An engineer with this email already exists.";
    const std::string INVALID_ID           = "ID must be a valid positive integer.";
}
```

### Database Layer (Database.h / Database.cpp)
- Open and manage a single SQLite connection
- Provide methods: `execute(sql)`, `query(sql)` returning rows as vector of maps
- `createTables()` — runs all CREATE TABLE IF NOT EXISTS statements on startup
- Constructor takes the DB path from Config.h

### Model Layer
Each model is a plain struct with:
- All fields matching the database schema
- A `toJson()` method returning an `nlohmann::json` object
- A static `fromRow()` factory method that builds the struct from a database row map

### Service Layer
Each service class takes a `Database` reference via constructor injection.
Services must never directly interact with HTTP request/response objects.

**TicketService must implement:**
- `getAllTickets(filters)` — supports optional search, status, priority, category filters
- `getTicketById(id)` → returns ticket or throws not-found error
- `createTicket(data)` → validates, inserts, creates activity log, returns new ticket
- `updateTicket(id, data)` → validates, updates, creates activity log, returns updated ticket
- `deleteTicket(id)` → deletes, creates activity log (log before delete)
- `updateStatus(id, newStatus)` → updates status field only, creates log
- `assignEngineer(id, engineerId)` → assigns engineer, creates log

**EngineerService must implement:**
- `getAllEngineers()` → returns engineers with their active ticket count appended
- `getEngineerById(id)` → returns engineer or throws not-found error
- `createEngineer(data)` → validates, checks email uniqueness, inserts

**LogService must implement:**
- `createLog(ticketId, action, description)` → inserts activity log
- `getAllLogs()` → returns all logs ordered by created_at DESC
- `getLogsByTicketId(ticketId)` → returns logs for one ticket

**ReportService must implement:**
- `getSummary()` → counts for total, open, in_progress, resolved, closed, critical
- `getByStatus()` → array of `{ status, count }`
- `getByPriority()` → array of `{ priority, count }`
- `getByCategory()` → array of `{ category, count }`
- `getByEngineer()` → array of `{ engineer_name, ticket_count }`

### Controller Layer
Each controller registers routes on a `crow::SimpleApp` reference.
Controllers must only:
- Parse HTTP request body/params
- Call the appropriate Service method
- Return HTTP response with proper status code and JSON body

Controllers must NOT contain business logic.

**HTTP Status Codes to use:**
- `200 OK` — successful GET, PUT, PATCH
- `201 Created` — successful POST
- `204 No Content` — successful DELETE
- `400 Bad Request` — validation failed
- `404 Not Found` — resource not found
- `500 Internal Server Error` — unexpected database error

### Validator.h / Validator.cpp
Implement these validation functions:
```cpp
bool isNotEmpty(const std::string& value);
bool isValidEmail(const std::string& email);
bool isValidPriority(const std::string& priority);
bool isValidStatus(const std::string& status);
bool isValidCategory(const std::string& category);
bool isValidId(int id);
```

### JsonHelper.h
Shared helper functions for building consistent JSON responses:
```cpp
// Returns: { "success": true, "data": <payload> }
nlohmann::json successResponse(const nlohmann::json& data);

// Returns: { "success": false, "error": <message> }
nlohmann::json errorResponse(const std::string& message);
```

### main.cpp
- Initialize Config
- Create Database instance and call `createTables()`
- Optionally run seed data if database is empty
- Instantiate all Services (pass Database by reference)
- Instantiate all Controllers (pass Services by reference)
- Register all routes on the Crow app
- Configure CORS headers to allow requests from `http://localhost:5173`
- Start server on `Config::PORT`

### CMakeLists.txt Requirements
The root `CMakeLists.txt` must:
- Require CMake 3.15+
- Set C++ standard to C++17
- Use FetchContent to download Crow, nlohmann/json, and Catch2 automatically
- Link against SQLite3 (system-installed)
- Define a separate `tests` build target using Catch2

---

## UNIT TESTS

Create unit tests using Catch2 v3 for the following:

### test_ticket_service.cpp
- `createTicket` with valid data returns a ticket with correct fields
- `createTicket` with missing title throws a validation error
- `getTicketById` with a non-existent ID throws a not-found error
- `updateStatus` with an invalid status value throws a validation error

### test_engineer_service.cpp
- `createEngineer` with valid data returns an engineer
- `createEngineer` with duplicate email throws an error
- `getAllEngineers` returns the correct ticket count per engineer

### test_validator.cpp
- `isValidEmail` returns false for invalid formats
- `isValidPriority` returns false for undefined priority values
- `isValidStatus` returns false for undefined status values

---

## FRONTEND IMPLEMENTATION REQUIREMENTS

### TypeScript Types (types/index.ts)
Define interfaces for all data models:
```typescript
export interface Ticket {
  id: number;
  title: string;
  description: string;
  requester_name: string;
  requester_email: string;
  category: string;
  priority: 'Low' | 'Medium' | 'High' | 'Critical';
  status: 'Open' | 'In Progress' | 'Resolved' | 'Closed';
  assigned_engineer_id: number | null;
  assigned_engineer_name?: string;
  resolution_notes: string | null;
  created_at: string;
  updated_at: string;
}

export interface Engineer {
  id: number;
  full_name: string;
  email: string;
  role: string;
  department: string;
  active_ticket_count?: number;
  created_at: string;
}

export interface ActivityLog {
  id: number;
  ticket_id: number;
  action: string;
  description: string;
  created_at: string;
}

export interface ReportSummary {
  total: number;
  open: number;
  in_progress: number;
  resolved: number;
  closed: number;
  critical: number;
}

export type Priority = 'Low' | 'Medium' | 'High' | 'Critical';
export type Status = 'Open' | 'In Progress' | 'Resolved' | 'Closed';
export type Category = 'Software Issue' | 'Hardware Issue' | 'Network Issue' | 'Account Access' | 'System Bug' | 'Client Request' | 'Others';
```

### axiosInstance.ts
```typescript
import axios from 'axios';

const api = axios.create({
  baseURL: import.meta.env.VITE_API_URL || 'http://localhost:8080',
  headers: { 'Content-Type': 'application/json' },
});

export default api;
```

### Pages

**Dashboard.tsx**
- Fetch `/api/reports/summary` for metric cards
- Show 6 metric cards: Total, Open, In Progress, Resolved, Closed, Critical
- Fetch `/api/tickets?limit=5` for recent tickets table
- Show loading spinner while fetching
- Show error state if fetch fails

**Tickets.tsx**
- Display all tickets in a table with columns: ID, Title, Requester, Priority, Status, Category, Assigned To, Created, Actions
- Search bar that filters by ID, title, or requester name
- Dropdown filters for Status, Priority, Category
- Each row has View, Edit, Delete action buttons
- Delete shows a confirmation dialog before calling API

**CreateTicket.tsx**
- Form with all required fields
- Dropdown selects for Category, Priority, Status
- Dropdown for assigning an engineer (populated from `/api/engineers`)
- Submit calls `POST /api/tickets`
- On success, redirect to `/tickets`
- Show inline validation error messages

**TicketDetails.tsx**
- Display all ticket fields in a clean detail layout
- Show assigned engineer name (if assigned)
- Show activity log timeline at the bottom of the page
- Quick action buttons: Edit, Change Status, Delete
- Change Status uses a dropdown modal calling `PATCH /api/tickets/:id/status`

**EditTicket.tsx**
- Pre-filled form with current ticket data
- Same fields as CreateTicket form
- Submit calls `PUT /api/tickets/:id`
- On success, redirect to ticket detail page

**Engineers.tsx**
- Two-column layout: left side shows engineer list, right side has "Add Engineer" form
- Engineer list shows: name, email, role, department, active ticket count
- Add engineer form with validation

**Reports.tsx**
- Display report data as simple bar charts or percentage tables
- Four sections: By Status, By Priority, By Category, By Engineer
- Use a simple chart library (Chart.js via CDN, or build plain CSS bar charts using Tailwind)

### Component Requirements

**StatusBadge.tsx**
Color mapping:
- Open → blue badge
- In Progress → yellow/amber badge
- Resolved → green badge
- Closed → gray badge

**PriorityBadge.tsx**
Color mapping:
- Low → green badge
- Medium → blue badge
- High → orange badge
- Critical → red badge

**LoadingSpinner.tsx**
- Simple centered spinning circle using Tailwind `animate-spin`

**ErrorMessage.tsx**
- Red bordered box showing the error message string

---

## DOCKER CONFIGURATION

### backend/Dockerfile
```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential cmake libsqlite3-dev git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build

EXPOSE 8080

CMD ["./build/service_ticket_backend"]
```

### frontend/Dockerfile
```dockerfile
FROM node:18-alpine AS builder
WORKDIR /app
COPY package*.json ./
RUN npm install
COPY . .
RUN npm run build

FROM nginx:alpine
COPY --from=builder /app/dist /usr/share/nginx/html
COPY nginx.conf /etc/nginx/conf.d/default.conf
EXPOSE 80
```

### docker-compose.yml
```yaml
version: '3.8'
services:
  backend:
    build: ./backend
    ports:
      - "8080:8080"
    volumes:
      - ./backend/data:/app/data
    healthcheck:
      test: ["CMD", "curl", "-f", "http://localhost:8080/api/health"]
      interval: 30s
      timeout: 10s
      retries: 3

  frontend:
    build: ./frontend
    ports:
      - "3000:80"
    depends_on:
      - backend
    environment:
      - VITE_API_URL=http://localhost:8080
```

---

## .gitignore
```
# Database file (never commit real data)
*.db
*.db-shm
*.db-wal

# Build output
/backend/build/
/frontend/dist/
/frontend/node_modules/

# Environment variables
.env

# IDE files
.vscode/settings.json
*.suo
*.user
```

---

## .env.example
```
# Backend
APP_PORT=8080
DB_PATH=./data/tickets.db

# Frontend
VITE_API_URL=http://localhost:8080
```

---

## POSTMAN COLLECTION

Create the file `postman/ServiceTicketSystem.postman_collection.json` with pre-built requests for all 17 API endpoints. Group requests into folders:
- Tickets (7 requests)
- Engineers (3 requests)
- Logs (2 requests)
- Reports (5 requests)
- Health Check (1 request)

Each request should include:
- The correct HTTP method and URL
- Example request body (for POST/PUT/PATCH)
- A short description

---

## README.md

Write a complete README with these sections:

### 1. Project Title and Description
### 2. Tech Stack Table
### 3. Features List
### 4. Screenshots *(placeholder section with text "Screenshots coming soon")*
### 5. Project Architecture Diagram (plain text diagram showing the layers)
### 6. Folder Structure
### 7. How to Run Locally
```bash
# 1. Build and run backend
cd backend
cmake -B build && cmake --build build
./build/service_ticket_backend

# 2. Seed sample data (optional but recommended for demo)
sqlite3 data/tickets.db < data/seed.sql

# 3. Run frontend
cd frontend
npm install
npm run dev
```
### 8. How to Run with Docker
```bash
docker compose up --build
# Frontend: http://localhost:3000
# Backend API: http://localhost:8080
```
### 9. How to Run Tests
```bash
cd backend
cmake -B build -DBUILD_TESTS=ON && cmake --build build
./build/tests/run_tests
```
### 10. API Endpoints Reference Table
### 11. Database Schema
### 12. Recommended Git Commit Message Format
```
feat: add ticket CRUD endpoints
feat: add engineer assignment workflow
feat: add activity log auto-creation
feat: add reports aggregation service
test: add unit tests for TicketService
feat: add React dashboard page
docker: add Dockerfile and compose config
docs: complete README
```
### 13. How I Explain This Project in an Interview

> "I built a C++ Service Ticket Management System that simulates how IT support teams manage
> client issues and service requests — which is actually very close to what Accenture does for
> its enterprise clients. The backend is written in C++17 using the Crow web framework, with
> SQLite for storage and a clean layered architecture — Database, Models, Services, and
> Controllers are all separated. The frontend is React with TypeScript and Tailwind CSS.
>
> I implemented full ticket CRUD, engineer assignment, status tracking, an activity log that
> records every action automatically, and a reports module with aggregated data. The whole
> system runs with a single `docker compose up --build` command.
>
> The project helped me apply object-oriented programming, REST API design, database
> integration, input validation, and software architecture in a real-world business context.
> If I were to extend it, the first thing I'd add is JWT-based authentication to secure the
> endpoints, and then a PostgreSQL migration for production-grade data handling."

---

## INTERVIEW PREPARATION CHECKLIST

Before your interview, make sure you can answer these questions confidently:

**Architecture questions:**
- [ ] Why did you separate Services from Controllers?
- [ ] Why is business logic not in main.cpp?
- [ ] What is the benefit of the Database abstraction layer?

**C++ specific questions:**
- [ ] Why C++17? What features did you use? (structured bindings, `if constexpr`, `std::string_view`)
- [ ] How does Crow handle concurrent HTTP requests?
- [ ] How did you handle memory management? (RAII, no raw `new`/`delete`)

**Database questions:**
- [ ] Why SQLite for this project? What would you use in production?
- [ ] How did you prevent SQL injection?
- [ ] What does `ON DELETE CASCADE` do in your logs table?

**API design questions:**
- [ ] Why did you use PATCH for status and assign instead of PUT?
- [ ] What HTTP status codes do you return and why?
- [ ] How would you add authentication to this API?

**Frontend questions:**
- [ ] Why TypeScript over plain JavaScript?
- [ ] How did you handle loading and error states?
- [ ] What is the role of `axiosInstance.ts`?

**Improvement / growth questions:**
- [ ] What would you add next? *(Answer: JWT auth, PostgreSQL, pagination, email notifications)*
- [ ] What was the hardest part to build? *(Be honest and specific)*
- [ ] How did you test your API? *(Answer: Postman collection — show the file)*

---

*This plan is optimized for Claude Code / GitHub Copilot Workspace / Codex. Paste the PROMPT HEADER first, then this full document.*
