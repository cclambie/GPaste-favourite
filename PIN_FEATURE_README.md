# GPaste Pin/Favourite Feature Implementation

This document describes the implementation of the pin/favourite feature for GPaste clipboard manager. This feature was requested in GitHub issues #217 and #372.

## Feature Overview

The pin feature allows users to:
- **Pin clipboard items** - Pinned items are protected from automatic removal when the history reaches its maximum size
- **Visual indication** - Pinned items display a star icon (★) that can be clicked to pin/unpin
- **Persistence** - Pinned state is saved to the history file and persists across sessions
- **Position at bottom** - When an item is pinned, it moves to the bottom of the clipboard list; when unpinned, it returns to position after the first (active) item

## Files Modified

### Core Data Layer (`src/daemon/tmp/`)

1. **gpaste-item.h / gpaste-item.c**
   - Added `pinned` field to `GPasteItemPrivate` structure
   - Added `g_paste_item_is_pinned()` getter function
   - Added `g_paste_item_set_pinned()` setter function

2. **gpaste-history.h / gpaste-history.c**
   - Added `g_paste_history_set_pinned()` - Sets pinned state and repositions item
   - Added `g_paste_history_is_pinned()` - Queries pinned state by UUID
   - Modified `g_paste_history_private_check_size()` - Protects pinned items from removal when history exceeds max size

3. **gpaste-file-backend.c**
   - Modified XML writer to include `pinned="true"` attribute for pinned items
   - Modified XML parser to read and restore pinned state from history file
   - Added `pinned` field to the parser's `Data` structure

### D-Bus API Layer (`src/libgpaste/gpaste/`)

4. **gpaste-gdbus-defines.h**
   - Added `G_PASTE_DAEMON_SET_PINNED` method definition
   - Added `G_PASTE_DAEMON_IS_PINNED` method definition
   - Added corresponding D-Bus interface XML

5. **gpaste-daemon.c**
   - Added `g_paste_daemon_private_set_pinned()` handler
   - Added `g_paste_daemon_private_is_pinned()` handler
   - Registered handlers in the method dispatcher

6. **gpaste-client.h / gpaste-client.c**
   - Added sync methods: `g_paste_client_set_pinned_sync()`, `g_paste_client_is_pinned_sync()`
   - Added async methods: `g_paste_client_set_pinned()`, `g_paste_client_is_pinned()`
   - Added finish methods: `g_paste_client_set_pinned_finish()`, `g_paste_client_is_pinned_finish()`

### UI Layer (`src/ui/`)

7. **gpaste-ui-pin-item.h / gpaste-ui-pin-item.c** (NEW FILES)
   - New action button widget for pin/unpin functionality
   - Displays star icon (non-starred-symbolic / starred-symbolic)
   - Toggles pinned state on click
   - Updates icon to reflect current state

8. **gpaste-ui-item-skeleton.h / gpaste-ui-item-skeleton.c**
   - Added `pin` widget to private structure
   - Added `g_paste_ui_item_skeleton_set_pinned()` to update pin icon state
   - Includes pin widget in action list

9. **gpaste-ui-item.c**
   - Added `g_paste_ui_item_on_pinned_ready()` callback
   - Queries pinned state when item is loaded

10. **meson.build**
    - Added `gpaste-ui-pin-item.c` and `gpaste-ui-pin-item.h` to build

## Build Instructions

### Prerequisites

Make sure you have the required dependencies installed:

```bash
# Debian/Ubuntu
sudo apt install meson ninja-build libglib2.0-dev libgtk-3-dev \
    libgcr-3-dev appstream-util gettext

# Fedora
sudo dnf install meson ninja-build glib2-devel gtk3-devel \
    gcr-devel appstream-util gettext
```

### Building

```bash
cd /home/ubuntu/gpaste

# Configure the build
meson setup builddir

# Build
ninja -C builddir

# Install (optional)
sudo ninja -C builddir install
```

### Testing

After building:

1. Restart the GPaste daemon:
   ```bash
   gpaste-client daemon-reexec
   ```

2. Open GPaste UI:
   ```bash
   gpaste-client ui
   ```

3. Test the pin feature:
   - Click the star icon next to any clipboard item to pin it
   - Pinned items show a filled star (★)
   - Unpinned items show an empty star (☆)
   - Pinned items move to the bottom of the list
   - Pinned items persist after restarting GPaste

## History File Format

Pinned items are stored with a `pinned="true"` attribute in the XML history file:

```xml
<item kind="Text" uuid="..." pinned="true">
    <value><![CDATA[pinned content]]></value>
</item>
```

## D-Bus API

New methods exposed via D-Bus:

- `SetPinned(uuid: string, pinned: boolean)` - Set the pinned state of an item
- `IsPinned(uuid: string) -> boolean` - Check if an item is pinned

## Known Limitations

1. **Phase 1 implementation** - This is mouse-only interaction; keyboard shortcuts may be added in a future phase
2. **First item cannot be pinned to bottom** - The first (active) item stays at position 0 even if pinned

## Testing Notes

- Verify that pinned items persist after daemon restart
- Test that pinned items are not removed when history reaches max size
- Verify that unpinning moves the item back to position 1 (after active item)
- Check that the star icon properly toggles between filled/unfilled states

## Contributing

For bug reports or feature suggestions related to this pin feature, please reference GitHub issues #217 and #372.
