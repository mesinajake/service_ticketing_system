# C++ Service Ticket Management System

An interview-ready full-stack project for IT support operations. The system manages service tickets, engineer assignments, activity logs, and reporting dashboards using a layered C++17 backend and a React TypeScript frontend.

## Tech Stack

| Layer | Technology |
| --- | --- |
| Backend | C++17, Crow, CMake, SQLite3, nlohmann/json |
| Frontend | React 18, TypeScript, Tailwind CSS, Vite, Axios, React Router |
| Tests | Catch2 v3 service and validator tests |
| DevOps | Docker, Docker Compose, Nginx |

## Features

- Dashboard metrics for total, open, in-progress, resolved, closed, and critical tickets
- Ticket CRUD with search and filters
- Engineer management with active ticket counts
- Ticket assignment and status update workflows
- Automatic activity logs for important ticket actions
- Reports grouped by status, priority, category, and engineer
- SQLite seed data for demo use

## Screenshots

Screenshots coming soon.

## Architecture

```text
React + TypeScript UI
        |
Axios API Client
        |
Crow Controllers
        |
Service Layer
        |
Database Abstraction
        |
SQLite
```

## Folder Structure

```text
backend/
  src/
    config/
    controllers/
    database/
    models/
    services/
    utils/
    main.cpp
  tests/
  data/seed.sql
frontend/
  src/
    api/
    components/
    pages/
    types/
postman/
docker-compose.yml
```

## Run Locally

```bash
cd backend
cmake -B build && cmake --build build
./build/service_ticket_backend
```

```bash
cd frontend
npm install
npm run dev
```

The Vite dev server proxies `/api` to `http://localhost:8080`.

## Run With Docker

```bash
docker compose up --build
```

- Frontend: `http://localhost:3000`
- Backend API: `http://localhost:8080`
- Health check: `http://localhost:8080/api/health`

## Run Tests

```bash
cd backend
cmake -B build -DBUILD_TESTS=ON && cmake --build build
./build/run_tests
```

## API Endpoints

| Method | Endpoint | Description |
| --- | --- | --- |
| GET | `/api/health` | Health check |
| GET | `/api/tickets` | List tickets with optional search/filter params |
| GET | `/api/tickets/:id` | Get one ticket |
| POST | `/api/tickets` | Create ticket |
| PUT | `/api/tickets/:id` | Update ticket |
| DELETE | `/api/tickets/:id` | Delete ticket |
| PATCH | `/api/tickets/:id/status` | Update ticket status |
| PATCH | `/api/tickets/:id/assign` | Assign ticket to engineer |
| GET | `/api/engineers` | List engineers |
| GET | `/api/engineers/:id` | Get one engineer |
| POST | `/api/engineers` | Create engineer |
| GET | `/api/logs` | List activity logs |
| GET | `/api/tickets/:id/logs` | List logs for one ticket |
| GET | `/api/reports/summary` | Dashboard summary |
| GET | `/api/reports/status` | Counts by status |
| GET | `/api/reports/priority` | Counts by priority |
| GET | `/api/reports/category` | Counts by category |
| GET | `/api/reports/engineers` | Counts by engineer |

## Database Schema

The backend creates `tickets`, `engineers`, and `activity_logs` tables on startup. Demo data lives in `backend/data/seed.sql` and is loaded automatically when the engineers table is empty.

## Recommended Commit Messages

```text
feat: add ticket CRUD endpoints
feat: add engineer assignment workflow
feat: add activity log auto-creation
feat: add reports aggregation service
test: add unit tests for TicketService
feat: add React dashboard page
docker: add Dockerfile and compose config
docs: complete README
```

## Interview Explanation

I built a C++ Service Ticket Management System that simulates how IT support teams manage client issues and service requests. The backend uses C++17, Crow, SQLite, and a clean layered architecture. Controllers handle HTTP, services own business logic, models represent data, and the database layer wraps SQLite access.

The frontend uses React with TypeScript and Tailwind CSS. It includes ticket CRUD, engineer assignment, status tracking, automatic activity logs, and reports. The whole system can run with a single `docker compose up --build` command.
