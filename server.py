#!/usr/bin/env python3
"""
Simple HTTP server for serving WebAssembly files.
Run this in the project directory after compiling the WASM module.
"""
import http.server
import socketserver
import os

PORT = 8080

class WasmHandler(http.server.SimpleHTTPRequestHandler):
    """HTTP handler with proper MIME types for WebAssembly."""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=os.path.dirname(os.path.abspath(__file__)) or '.', **kwargs)

    def end_headers(self):
        # Add CORS headers for local development
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        super().end_headers()

    extensions_map = {
        '.html': 'text/html',
        '.js': 'application/javascript',
        '.wasm': 'application/wasm',
        '.css': 'text/css',
        '.json': 'application/json',
        '.png': 'image/png',
        '.jpg': 'image/jpeg',
        '.svg': 'image/svg+xml',
        '': 'application/octet-stream',
    }

if __name__ == '__main__':
    with socketserver.TCPServer(("", PORT), WasmHandler) as httpd:
        print(f"Power Calculator WebAssembly Server")
        print(f"=" * 40)
        print(f"Serving at http://localhost:{PORT}")
        print(f"Open http://localhost:{PORT}/index.html in your browser")
        print(f"Press Ctrl+C to stop")
        print(f"=" * 40)
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped.")
