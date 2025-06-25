# Custom File System in C

A fully custom file system written in C with:
- A bitmap-based block allocator
- Directory structures with nested subfolders
- Virtual disk image file (`disk.img`)
- Superblock, file table, and block-level file access

---

## Build and Run

### Requirements
- GCC or Clang
- Unix-based OS (Linux, macOS, WSL)

### Build

```bash
make
```

### Run

```bash
./build/fs
```

---

## Disk Layout

```
Block 0    → Superblock
Block 1    → Bitmap
Block 2    → Root directory
Block 3+   → Files, subdirectories, etc.
```

---

## Roadmap

- [ ] Virtual disk image (`disk.img`) as storage backend
- [ ] Bitmap-based block allocation
- [ ] Hierarchical directory structure (like `/home/user/docs`)
- [ ] FileEntry structure for tracking file metadata
- [ ] Superblock with file system metadata
- [ ] Set/clear/check bits for block allocation
- [ ] File reading/writing
- [ ] File deletion and defragmentation
- [ ] Mounting and unmounting logic
- [ ] Real command-line interface (like `mkdir`, `ls`, `cat`)
- [ ] A TUI / GUI for file handling
- [ ] Support for file contents (read/write)
- [ ] File permissions and timestamps
- [ ] Journaling for crash recovery
- [ ] Mountable FUSE file system (Linux)

---

## Author

**Adarsh Ravikumar**
---

## License

MIT

---

## 📍 Inspiration

- MINIX file system
- FAT32 + EXT2
- Operating Systems: Three Easy Pieces
