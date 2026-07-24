# postgres-debugger Docker image

PostgreSQL images with the [pldebugger](https://github.com/ng-galien/pldebugger)
extension preinstalled and preconfigured, ready to debug PL/pgSQL functions from
pgAdmin or any client speaking the `pldbgapi` protocol.

This build uses the `print-vars` branch of the fork, which adds enhanced variable
printing (RECORD type support) on top of the upstream
[EnterpriseDB/pldebugger](https://github.com/EnterpriseDB/pldebugger) extension.

Images are published on Docker Hub as
[`galien0xffffff/postgres-debugger`](https://hub.docker.com/r/galien0xffffff/postgres-debugger).

## Supported tags

PostgreSQL **13 to 18**, on `linux/amd64` and `linux/arm64`:

| Tag | Base image |
|-----|------------|
| `13` … `18` | `postgres:<version>-trixie` (alias of the trixie variant) |
| `13-trixie` … `18-trixie` | Debian 13 (trixie) |
| `13-bookworm` … `18-bookworm` | Debian 12 (bookworm) |

Prefer the short tags (`17`, `18`, …): they track the most recent Debian base.
Images are rebuilt and republished monthly (the 23rd) so the base images pick up
security updates.

## Usage

```bash
docker run -d --name pg-debug -p 5432:5432 \
  -e POSTGRES_PASSWORD=postgres \
  galien0xffffff/postgres-debugger:18
```

The image already sets `shared_preload_libraries = 'plugin_debugger'` at initdb
time (see `config.sh`). To start debugging, create the extension in the target
database:

```sql
CREATE EXTENSION IF NOT EXISTS pldbgapi;
```

Then attach a graphical debugger (pgAdmin: right-click a function → *Debugging*).

## What the build does

`Dockerfile` starts from the official `postgres:<BASE_IMAGE>` image and:

1. installs the build toolchain and `postgresql-server-dev-<TAG>`;
2. clones the matching `REL_<TAG>_STABLE` branch of PostgreSQL (needed as build
   tree for PGXS) and the `print-vars` branch of this repository into `contrib/`;
3. builds and installs the extension with `make USE_PGXS=1 install`;
4. removes sources and the toolchain to keep the image small;
5. drops `config.sh` into `/docker-entrypoint-initdb.d/` to enable
   `plugin_debugger` automatically on first startup.

## Building locally

```bash
export PG_VERSION=18 \
  && export PG_PLATFORM=linux/arm64 \
  && export PG_IMAGE=postgres-debugger \
  && export DOCKER_USER=galien0xffffff \
  && export BASE_IMAGE="${PG_VERSION}-trixie"

docker buildx build --platform $PG_PLATFORM \
  --build-arg "TAG=$PG_VERSION" \
  --build-arg "BASE_IMAGE=$BASE_IMAGE" \
  -t "$DOCKER_USER/$PG_IMAGE:$PG_VERSION" .
```

## CI

Publication is automated by
[`.github/workflows/docker-publish.yaml`](../.github/workflows/docker-publish.yaml):
a monthly cron (plus manual `workflow_dispatch`) builds the full
version × variant matrix multi-arch and pushes to Docker Hub. A keepalive job
prevents GitHub from disabling the schedule after 60 days of repository
inactivity.
