# Surveillance Monitoring Demo

This independent subsystem demonstrates a realistic physical-security event
flow without changing `qmlmqttdemo`.

```text
MQTT camera event -> security-server -> parse/rules/GIS -> PostgreSQL
                                                   -> NATS -> operator-client
```

## Components

- `security-server`: MQTT ingestion, worker-thread parsing/rules, PostgreSQL
  persistence, and NATS incident publishing.
- `operator-client`: Qt/QML dummy operator dashboard that subscribes to NATS
  and displays live incidents with GIS coordinates and resolved zone.
- `common`: shared domain types and the demo GIS geofence rule.
- `tests`: parser/rule unit tests.

The demo maps events inside a small fictional Athens perimeter to
`restricted-perimeter`. A `motion` event there becomes a `high` severity
incident; `tamper` and `offline` become `critical`.

## Run locally

Start the required infrastructure:

```bash
cd surveillance
docker compose up -d
```

Configure and build from the repository root. Point `CMAKE_PREFIX_PATH` at the
Qt installation if it is not already discoverable.

```bash
cmake -S . -B build
cmake --build build
```

Run these in separate terminals:

```bash
./build/surveillance/server/security-server
./build/surveillance/client/operator-client
```

Publish the sample camera event:

```bash
docker compose -f surveillance/docker-compose.yml exec -T mosquitto \
  mosquitto_pub -h localhost -t cameras/cam-athens-01/events \
  -m "$(tr -d '\n' < surveillance/examples/motion_event.json)"
```

The client should immediately display a high-severity incident. The server
defaults are configurable with `MQTT_HOST`, `MQTT_PORT`, `MQTT_TOPIC`,
`NATS_HOST`, `NATS_PORT`, and `DATABASE_URL`.

## Deliberate boundaries

`EventProcessor` and `DataStore` live on dedicated worker threads. MQTT and
NATS use asynchronous Qt sockets/client APIs on the application event loop.
The database is the durable source of history; NATS is the live update path.

The NATS integration is a minimal implementation of the NATS text protocol for
this demo. It supports the necessary `CONNECT`, `PING`, `SUB`, and `PUB`
commands but intentionally does not yet implement TLS, authentication,
reconnect/backoff, JetStream durability, or outbound replay. Those are the next
production-hardening milestones.
