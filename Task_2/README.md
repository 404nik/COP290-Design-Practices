# COP290_Game

## How to Compile and Run the Code

### Server
1. Compile the server:
   ```sh
   make -f makeS
   ```
2. Run the server with the default health port (`8080`):
   ```sh
   ./playS 51717
   ```
   Or choose an explicit health port for monitoring/tests:
   ```sh
   ./playS 51717 18080
   ```
3. Press `Enter`.

The first argument is always the game port used by `playC`. The optional second argument is a separate unauthenticated HTTP health port; keep it different from the game port.

### Health probes

`playS` runs a small HTTP health listener on loopback (`127.0.0.1`) independently of the game socket.

```sh
curl -i http://127.0.0.1:8080/health/live
curl -i http://127.0.0.1:8080/health/ready
```

Response contract:

| Route | When | Status | Body |
| --- | --- | --- | --- |
| `GET /health/live` | Health listener can serve HTTP | `200 OK` | `{"status":"ok"}` |
| `GET /health/ready` | Game socket is not listening yet or server is shutting down | `503 Service Unavailable` | `{"status":"not_ready"}` |
| `GET /health/ready` | SDL/resources initialized and game socket `bind`/`listen` succeeded | `200 OK` | `{"status":"ready"}` |
| Unsupported path | Any time | `404 Not Found` | `{"status":"not_found"}` |
| Unsupported method | Any time | `405 Method Not Allowed` | `{"status":"method_not_allowed"}` |

Every response uses `Content-Type: application/json`, an exact `Content-Length`, and `Connection: close`.

Monitoring guidance:

- Use `/health/live` for process-restart decisions only.
- Use `/health/ready` for readiness or traffic-routing decisions because it turns `ready` only after the game socket is actually listening.
- Alert on sustained failures instead of a single missed probe.
- The health checks do not inspect player count, assets, audio devices, external files, or gameplay state.
- Keep the health port separate from the game port and restrict access to trusted operators/monitoring systems because the endpoint is deliberately unauthenticated.

### Client
1. Compile the client:
   ```sh
   make -f makeC
   ```
2. Run the client:
   ```sh
   ./playC localhost 51717
   ```
3. Press `Enter` to start the game.

## References Used for Codes

- [SDL Tutorials by Lazy Foo](https://lazyfoo.net/tutorials/SDL/index.php)
- [Google Images](https://www.google.co.in/imghp?hl=en&ogbl)

## Instructions for Running the Game

1. **Start the Game**: 
   - First, compile and run the server.
   - Then, compile and run the client.

2. **Gameplay**:
   - Move the character to the tasks generated at random locations.
   - Use the Yulu bike to do tasks efficiently by going to the bike stand and pressing `SPACE` to grab it.
   - Go to images generated on the map and press `t` to do the corresponding task.
   - Once a task is completed, the image will change to indicate completion.
   - Complete all 12 tasks generated randomly on the entire map.
   - The player who completes all the tasks first will be the winner.
   - Learn about various buildings by going to them and pressing `ENTER`.

Enjoy the game!
