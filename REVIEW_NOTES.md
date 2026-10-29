# Review notes

## Changes applied to both copies
- Added `TransferProtocol.h` helpers to fully transmit/receive fixed-size headers and send complete payload chunks.
- Validate header length and nonnegative file size; sanitize received filename to a basename.
- Detect early disconnects and failed sends; avoid reporting a truncated receive as success.
- Bound reads to remaining file size and avoid division by zero in progress display.
- Removed misleading hardcoded claim that an optimized route was selected.
- Added missing-router guard in the simulation engine.

## Remaining limitations
- Not compiled or run here: project depends on Windows Winsock/MSVC. Verify on Windows before publishing as tested.
- The raw `FileHeader` binary layout is compiler/ABI-dependent; use explicit byte-order serialization for cross-platform communication.
- Error paths in the legacy programs may return before closing all sockets; consider RAII cleanup.
- Routing simulation algorithms are not yet integrated into the actual TCP forwarding path. Flooding implementation builds neighbor entries but does not flood packets through the network.
- The simulator accumulates route-table destination costs as if each were a single-hop edge weight; metrics may be inaccurate.
- The two supplied ZIP archives contain matching files; both are retained at user request.
