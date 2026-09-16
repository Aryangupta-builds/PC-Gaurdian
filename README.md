# 🛡️ PC Guardian

### Smart File & System Health Manager

PC Guardian is a C++ command-line project focused on scanning files and analyzing storage information through a clean terminal interface.

> 🚧 **Project Status: Under Active Development**
>
> This project is currently being built and tested. This README describes the **current implemented progress only**.

---

## 🤖 README Note

This README file was **AI-generated**.

---

## 🛠️ Tech Stack

- **Language:** C++
- **C++ Standard Library:** STL
- **Filesystem:** `std::filesystem`
- **Containers:** `std::vector`, `std::map`
- **Algorithms:** `std::sort`
- **Formatting:** `<iomanip>`, `<format>`
- **CLI:** Windows Terminal / Console
- **Version Control:** Git

---

# 📊 Current Progress

## V1 — Basic File Scanner ✅

Implemented:

- Directory path input
- Directory validation
- File scanning
- File information collection
- `FileInfo` data structure
- File name, extension and size tracking
- Professional CLI table formatting
- Total file count
- Total storage size
- Largest file detection
- Extension-wise file counting
- Extension-wise storage analysis
- Filesystem error handling
- Invalid path handling
- Exception handling using `try/catch`

---

## V2 — Storage Analyzer

### M1 — Storage Data Collection ✅

Implemented:

- Total file count
- Total folder count
- Total storage used
- Storage analysis using collected file data

### M2 — Largest Files ✅

Implemented:

- Largest file detection
- Top 5 largest files
- File-size comparison
- Descending file-size sorting
- Sorting using a copied vector while preserving the original scan order

### CLI Visualization ✅

Implemented:

- Dynamic scanning progress bar
- Static storage distribution bars
- Percentage-based storage visualization
- Human-readable file sizes
- Dynamic table-width calculation

---

# V3 — Deep System Scanner

## M1 — Recursive Directory Scanning 🚧

Currently being implemented and tested.

Current implementation includes:

- Recursive directory traversal using `std::filesystem::recursive_directory_iterator`
- Scanning files inside nested directories
- Tracking each file's relative location
- Storing relative paths inside `FileInfo`
- Displaying file location in the CLI
- Dynamic location-column width handling

---

# 🖥️ Current CLI

PC Guardian currently provides terminal-based information such as:

```text
TOP LARGE FILE

Name                     Location          Extension          Size       Category
---------------------------------------------------------------------------------
DC-movie.mkv             test              .mkv               1.97GB     Large
test2.txt                test              .txt               20.00MB    Small
test.txt                 test              .txt               20.00MB    Small
sub folder file.txt      test\empty-test   .txt               20.00MB    Small
