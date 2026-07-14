"""
server.py  —  Campus Route Planner Backend Server

Run:   python server.py
Open:  http://localhost:5000
"""

import subprocess, json, os, sys
from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse
import re

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
CPP_BIN  = os.path.join(BASE_DIR, "api_backend.exe" if sys.platform == "win32" else "api_backend")

def run_cpp(*args):
    cmd = [CPP_BIN] + [str(a) for a in args]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, cwd=BASE_DIR, timeout=10)
        lines = [l.strip() for l in result.stdout.strip().splitlines() if l.strip()]
        if not lines:
            return {"error": "No output from C++ backend"}
        return json.loads(lines[-1])
    except subprocess.TimeoutExpired:
        return {"error": "C++ backend timed out"}
    except json.JSONDecodeError as e:
        return {"error": f"Bad JSON from C++: {str(e)}"}
    except Exception as e:
        return {"error": str(e)}

class Handler(BaseHTTPRequestHandler):

    def log_message(self, format, *args):
        print(f"  [{args[1]}] {args[0]}")

    def send_json(self, data, status=200):
        body = json.dumps(data).encode()
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def send_file(self, path, mime):
        with open(path, "rb") as f:
            body = f.read()
        self.send_response(200)
        self.send_header("Content-Type", mime)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_OPTIONS(self):
        self.send_response(200)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.end_headers()

    def handle_api(self, path, method):
        # GET /api/locations
        if path == "/api/locations":
            return self.send_json(run_cpp("locations"))

        # GET /api/graph
        if path == "/api/graph":
            return self.send_json(run_cpp("graph"))

        # GET /api/shortest/1/16
        m = re.match(r"^/api/shortest/(\d+)/(\d+)$", path)
        if m:
            return self.send_json(run_cpp("shortest", m[1], m[2]))

        # GET /api/bfs/1
        m = re.match(r"^/api/bfs/(\d+)$", path)
        if m:
            return self.send_json(run_cpp("bfs", m[1]))

        # GET /api/dfs/1
        m = re.match(r"^/api/dfs/(\d+)$", path)
        if m:
            return self.send_json(run_cpp("dfs", m[1]))

        # GET /api/emergency/1
        m = re.match(r"^/api/emergency/(\d+)$", path)
        if m:
            return self.send_json(run_cpp("emergency", m[1]))

        # GET /api/routes/1/16
        m = re.match(r"^/api/routes/(\d+)/(\d+)$", path)
        if m:
            return self.send_json(run_cpp("routes", m[1], m[2]))

        # GET /api/connectivity/1/16
        m = re.match(r"^/api/connectivity/(\d+)/(\d+)$", path)
        if m:
            return self.send_json(run_cpp("connectivity", m[1], m[2]))

        # POST /api/block/1/2
        m = re.match(r"^/api/block/(\d+)/(\d+)$", path)
        if m:
            return self.send_json(run_cpp("block", m[1], m[2]))

        # POST /api/unblock/1/2
        m = re.match(r"^/api/unblock/(\d+)/(\d+)$", path)
        if m:
            return self.send_json(run_cpp("unblock", m[1], m[2]))

        self.send_json({"error": "Unknown endpoint"}, 404)

    def do_GET(self):
        path = urlparse(self.path).path

        if path.startswith("/api/"):
            return self.handle_api(path, "GET")

        # Serve index.html for /
        if path == "/" or path == "/index.html":
            html_path = os.path.join(BASE_DIR, "index.html")
            return self.send_file(html_path, "text/html; charset=utf-8")

        self.send_json({"error": "Not found"}, 404)

    def do_POST(self):
        path = urlparse(self.path).path
        if path.startswith("/api/"):
            return self.handle_api(path, "POST")
        self.send_json({"error": "Not found"}, 404)


if __name__ == "__main__":
    print("\n" + "="*55)
    print("  Campus Route Planner — Backend Server")
    print("  No extra packages needed!")
    print("="*55)

    if not os.path.exists(CPP_BIN):
        print(f"\n  [ERROR] C++ binary not found!")
        print(f"  Please compile first by running:")
        print(f"    g++ -o api_backend.exe api_backend.cpp")
        sys.exit(1)

    print(f"\n  C++ backend  : {CPP_BIN}  [FOUND]")
    print(f"  Server URL   : http://localhost:5000")
    print(f"\n  Open your browser and go to:")
    print(f"  http://localhost:5000")
    print(f"\n  Press Ctrl+C to stop.")
    print("="*55 + "\n")

    server = HTTPServer(("localhost", 5000), Handler)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n  Server stopped.")
