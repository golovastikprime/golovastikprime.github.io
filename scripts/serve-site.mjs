import { createReadStream } from 'node:fs';
import { realpath, stat } from 'node:fs/promises';
import { createServer } from 'node:http';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(fileURLToPath(new URL('..', import.meta.url)));
const HOST = '127.0.0.1';

function requestedPort() {
  const args = process.argv.slice(2);
  if (args.includes('--help') || args.includes('-h')) {
    console.log('Usage: node scripts/serve-site.mjs [--port PORT]');
    process.exit(0);
  }

  let value = process.env.PORT ?? '8000';
  if (args.length === 1 && /^\d+$/.test(args[0])) {
    [value] = args;
  } else if (args.length === 2 && args[0] === '--port') {
    [, value] = args;
  } else if (args.length > 0) {
    throw new Error('Usage: node scripts/serve-site.mjs [--port PORT]');
  }

  const port = Number(value);
  if (!Number.isInteger(port) || port < 1 || port > 65535) {
    throw new Error(`Invalid port: ${value}`);
  }
  return port;
}

const PORT = requestedPort();
const REAL_ROOT = await realpath(ROOT);

const CONTENT_TYPES = new Map([
  ['.css', 'text/css; charset=utf-8'],
  ['.gif', 'image/gif'],
  ['.html', 'text/html; charset=utf-8'],
  ['.ico', 'image/x-icon'],
  ['.jpeg', 'image/jpeg'],
  ['.jpg', 'image/jpeg'],
  ['.js', 'text/javascript; charset=utf-8'],
  ['.json', 'application/json; charset=utf-8'],
  ['.mjs', 'text/javascript; charset=utf-8'],
  ['.pdf', 'application/pdf'],
  ['.png', 'image/png'],
  ['.svg', 'image/svg+xml'],
  ['.txt', 'text/plain; charset=utf-8'],
  ['.webmanifest', 'application/manifest+json'],
  ['.xml', 'application/xml; charset=utf-8'],
]);

function send(response, status, message, extraHeaders = {}) {
  const body = `${message}\n`;
  response.writeHead(status, {
    'Cache-Control': 'no-store',
    'Content-Length': Buffer.byteLength(body),
    'Content-Type': 'text/plain; charset=utf-8',
    'X-Content-Type-Options': 'nosniff',
    ...extraHeaders,
  });
  response.end(body);
}

function isInsideRoot(filePath) {
  const relative = path.relative(REAL_ROOT, filePath);
  return relative === '' || (!relative.startsWith(`..${path.sep}`) && relative !== '..' && !path.isAbsolute(relative));
}

async function findFile(candidate) {
  try {
    const details = await stat(candidate);
    if (details.isDirectory()) return findFile(path.join(candidate, 'index.html'));
    if (details.isFile()) return candidate;
  } catch {
    if (!path.extname(candidate)) {
      try {
        const htmlCandidate = `${candidate}.html`;
        const details = await stat(htmlCandidate);
        if (details.isFile()) return htmlCandidate;
      } catch {
        return null;
      }
    }
  }
  return null;
}

const server = createServer(async (request, response) => {
  if (request.method !== 'GET' && request.method !== 'HEAD') {
    send(response, 405, 'Method Not Allowed', { Allow: 'GET, HEAD' });
    return;
  }

  let pathname;
  try {
    pathname = decodeURIComponent(new URL(request.url ?? '/', `http://${HOST}`).pathname);
  } catch {
    send(response, 400, 'Bad Request');
    return;
  }

  if (
    pathname.includes('\0')
    || pathname.split('/').some((segment) => segment.startsWith('.') && segment !== '')
  ) {
    send(response, 404, 'Not Found');
    return;
  }

  const candidate = path.resolve(REAL_ROOT, `.${pathname}`);
  if (!isInsideRoot(candidate)) {
    send(response, 403, 'Forbidden');
    return;
  }

  const filePath = await findFile(candidate);
  if (!filePath) {
    send(response, 404, 'Not Found');
    return;
  }

  let realFilePath;
  let details;
  try {
    realFilePath = await realpath(filePath);
    if (!isInsideRoot(realFilePath)) {
      send(response, 403, 'Forbidden');
      return;
    }
    details = await stat(realFilePath);
  } catch {
    send(response, 404, 'Not Found');
    return;
  }

  response.writeHead(200, {
    'Cache-Control': 'no-store',
    'Content-Length': details.size,
    'Content-Type': CONTENT_TYPES.get(path.extname(realFilePath).toLowerCase()) ?? 'application/octet-stream',
    'X-Content-Type-Options': 'nosniff',
  });

  if (request.method === 'HEAD') {
    response.end();
    return;
  }

  const stream = createReadStream(realFilePath);
  stream.on('error', () => response.destroy());
  stream.pipe(response);
});

server.on('error', (error) => {
  console.error(`Could not start server: ${error.message}`);
  process.exitCode = 1;
});

server.listen(PORT, HOST, () => {
  console.log(`Serving ${ROOT} at http://${HOST}:${PORT}/`);
});

for (const signal of ['SIGINT', 'SIGTERM']) {
  process.on(signal, () => server.close(() => process.exit(0)));
}
