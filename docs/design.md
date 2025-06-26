## Formatting
- `s`  => Size of disk (in bytes)
- `b`  => Size of 1 Block (in bytes)
- `i`  => Size of 1 Inode (in bytes)
- `t`  => Total Number Of Blocks
- `nb` => Number of blocks required for Bitmap
- `ni` => Number of blocks required for Inode
- `nd` => Number of blocks required for data

---

- `t = s / b`
- `nb = t / (8 * b)`
- `ni = t / (16 * i)`
- `nd = t - (nb + ni + 1)`

---

- `BITMAP_START` = 1
- `BITMAP_END`   = `BITMAP_START` + `nb`
- `INODE_START`  = `BITMAP_END` + 1
- `INODE_END`    = `INODE_START` + `ni`
- `DATA_START`   = `INODE_END` + 1
- `DATA_END`     = `DATA_START` + `nd`

---

## Layout

`[ Metablock ] [ Bitmap ] [ Inode Table ] [ Data ]`

- Block 0                                => Metablock
- Block `BITMAP_START` to  `BITMAP_END`  => Bitmap
- Block `INODE_START`  to  `INODE_END`   => Inode Table
- Block `DATA_START`   to  `DATA_END`    => Data

---

## Metablock
- 32 bit magic number   => `0xCOFFEE`
- 32 bit block size => `b  (0x1000)`
- 32 bit num blocks => `t`
- 32 bit bitmap start => `BITMAP_START`
- 32 bit inode start => `INODE_START`
- 32 bit inode size => `i`
- 32 bit data start => `DATA_START`
- 32 bit root inode id => `0x1`
- 256 bit drive name => `char[32]`

Total size = `64 bytes`
Free space left in block = `4032 bytes` (Will be used for metadata only journaling)

---

## Bitmap
- 32 bit array of length `t/32` spanning `nb` blocks

Total size = `nb * b bytes`

---

## Inode
- 8 bit flag
    - [ reserved ] [ reserved ] [ reserved ] [ exec ] [ system ] [ readonly ] [ hidden ] [ dir ] 

- 32 bit size
- 32 bit id
- 32 bit parent id

- 16 bit permissions
- 32 bit user id
- 32 bit group id

- 32 bit create time
- 32 bit access time
- 32 bit modify time

- 32 bit array of length 19 for direct blocks   (76 KB of data)
- 32 bit indirect block pointer                 (4  MB of data)
- 32 bit double indirect block pointer.         (4  GB of data)
- 32 bit tripple indirect block pointer.        (4  TB of data)

Size of 1 inode = `128 bytes`
Total size of all required inodes = `t / 2048`

---

## DirEntry
- 32 bit inode id
- 480 bit name => `char[60]`

Total size = 64 bytes

---

## User Story

1. **Creating a Folder**
   - A new **inode** is allocated and marked as a directory.
   - A free **data block** is selected via the bitmap and assigned as the folder's first block (direct block).
   - The inode's `direct[0]` points to this block.
   - The inode's `parent_id` points to the parent directory.
   - A `DirEntry` is created and written to the parent directory's data block, referencing the new inode ID.

2. **Creating a File Inside a Folder**
   - A new inode is allocated, with flags set appropriately (not a directory).
   - Required data blocks are allocated and linked using:
     - Direct pointers first
     - Then indirect → double indirect → triple indirect if needed
   - File contents are written into these data blocks.
   - A `DirEntry` is created in the parent folder (as above), pointing to the new inode.

3. **Reading a File**
   - The path is traversed using the root inode.
   - Each directory block is parsed for `DirEntry` structures.
   - The matching entry's inode is loaded.
   - The file is read via direct/indirect/double indirect pointers from the inode.

4. **Handling Directories**
   - Directory data blocks contain a sequence of `DirEntry` structures.
   - These entries reference inodes of contained files/folders.
   - Directories themselves are files with the `dir` flag set.

5. **Handling `.` and `..`**
   - Each directory contains two special entries:
     - `.` pointing to its own inode
     - `..` pointing to its parent inode (or to self in case of root)
   - These are added automatically when a directory is created.

6. **Filesystem Mounting**
   - On mount, the metablock (block 0) is read.
   - The layout is initialized: bitmap, inode table, and data start are located.
   - Root inode (ID `0x1`) is used as the entry point.

7. **Journaling (Metadata-Only)**
   - Journal entries are written into the unused metablock space.
   - Each entry records operations like "create file", "delete inode", etc.
   - On startup, journal is scanned for incomplete operations and rolled back or finalized.