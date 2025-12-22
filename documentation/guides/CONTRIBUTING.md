# Contributing to Naghuma Toolbox

Thank you for your interest in contributing to Naghuma Toolbox! This document provides guidelines for contributing to the project.

## ?? How to Contribute

### Reporting Bugs

Before creating a bug report, please check existing issues. When creating a bug report, include:

- **Clear title** describing the issue
- **Steps to reproduce** the problem
- **Expected behavior**
- **Actual behavior**
- **Screenshots** if applicable
- **Environment details** (OS, Qt version, OpenCV version)

### Suggesting Enhancements

Enhancement suggestions are welcome! Please provide:

- **Clear use case** for the enhancement
- **Expected behavior** after implementation
- **Alternative solutions** you've considered
- **Impact** on existing features

### Pull Requests

1. **Fork** the repository
2. **Create a branch** from `master`:
   ```bash
   git checkout -b feature/YourFeatureName
   ```
3. **Make your changes**:
   - Follow existing code style
   - Add comments for complex logic
   - Update documentation if needed
4. **Test thoroughly**:
   - Test with different image types
   - Verify layer system integration
   - Check for memory leaks
5. **Commit** with clear messages:
   ```bash
   git commit -m "feat: Add new filter operation"
   ```
6. **Push** to your fork:
   ```bash
   git push origin feature/YourFeatureName
   ```
7. **Open a Pull Request** with description of changes

## ?? Code Style Guidelines

### C++ Style

- **C++17** standard
- **Qt naming conventions**: 
  - camelCase for functions/variables
  - PascalCase for classes
- **OpenCV conventions**:
  - cv::Mat for images
  - Avoid raw pointers, use smart pointers
- **Comments**:
  - Doxygen-style for public APIs
  - Inline comments for complex algorithms

### Example:

```cpp
/**
 * @brief Apply custom filter to image
 * @param input Input image (BGR or grayscale)
 * @param output Output filtered image
 * @param kernelSize Size of filter kernel
 * @return true if successful
 */
bool applyCustomFilter(const cv::Mat& input, cv::Mat& output, int kernelSize) {
    if (input.empty()) return false;
    
    // Apply filter
    cv::GaussianBlur(input, output, cv::Size(kernelSize, kernelSize), 0);
    
    return true;
}
```

### File Organization

- **Headers** in `include/`
- **Source** in `src/`
- **Phase handlers** as `MainWindow_PhaseX.cpp`
- **Libraries** in `lib/category/`
- **Dialogs** as `XDialog.h` + `XDialog.cpp`

## ??? Architecture Guidelines

### Adding New Operations

1. **Create operation function**:
   ```cpp
   auto operation = [params](const cv::Mat& input) -> cv::Mat {
       cv::Mat result;
       // Your operation here
       return result;
   };
   ```

2. **Add to layer system**:
   ```cpp
   rightSidebar->addLayer("Operation Name", "category", 
                         processedImage, operation);
   ```

3. **Add menu item**:
   ```cpp
   ADD_MENU_ACTION(menu, "Operation Name...", handlerFunction);
   ```

### Dialog Pattern

All dialogs should follow this pattern:

```cpp
class CustomDialog : public QDialog {
    Q_OBJECT
    
public:
    CustomDialog(const cv::Mat& image, QWidget* parent = nullptr);
    cv::Mat getProcessedImage() const { return processedImage; }
    bool wasApplied() const { return applied; }
    
private slots:
    void onApply();
    void onPreview();
    
private:
    void setupUI();
    cv::Mat originalImage;
    cv::Mat processedImage;
    bool applied;
};
```

## ?? Testing

### Manual Testing Checklist

- [ ] Load different image formats (PNG, JPG, BMP, TIFF)
- [ ] Test with grayscale and color images
- [ ] Test with small (100×100) and large (4K+) images
- [ ] Verify layer system integration
- [ ] Check undo/redo functionality
- [ ] Test edge cases (empty image, single pixel, etc.)
- [ ] Verify memory usage with Task Manager
- [ ] Check for memory leaks

### Performance Guidelines

- Operations should complete in < 100ms for 1MP images
- Show progress bar for operations > 1 second
- Use Qt Concurrent for heavy computations (future)
- Optimize with OpenCV's parallel_for_ when applicable

## ?? Documentation

### Code Documentation

- **Public APIs** need Doxygen comments
- **Complex algorithms** need inline comments
- **Phase handlers** need file header comment

### Example Header:

```cpp
// ============================================================================
// MAINWINDOW - PHASE X: FEATURE NAME
// ============================================================================
// Purpose: Brief description of feature
// Features: List of key features
// Dependencies: Required libraries/modules
// ============================================================================
```

### User Documentation

When adding new features:
- Update README.md with feature description
- Add to User Guide section
- Include keyboard shortcuts if applicable
- Add screenshots to docs/screenshots/

## ?? Priority Areas

We especially welcome contributions in:

1. **Performance optimization**
   - Multi-threading support
   - GPU acceleration (CUDA/OpenCL)
   - Memory optimization

2. **New algorithms**
   - Additional segmentation methods
   - More filters
   - Advanced transforms

3. **UI/UX improvements**
   - Keyboard shortcuts
   - Accessibility features
   - Theme customization

4. **Documentation**
   - Tutorial videos
   - API documentation
   - Translation to other languages

5. **Testing**
   - Unit tests
   - Integration tests
   - Performance benchmarks

## ?? Commit Message Format

Use conventional commit format:

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Types:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting)
- `refactor`: Code refactoring
- `perf`: Performance improvements
- `test`: Adding tests
- `chore`: Build process or auxiliary tool changes

### Example:

```
feat(filters): Add bilateral filter with adjustable parameters

- Implement bilateral filter dialog
- Add real-time preview
- Integrate with layer system
- Update documentation

Closes #42
```

## ?? What NOT to Contribute

- Binary files (executables, DLLs)
- Build artifacts (Debug/, Release/, x64/)
- IDE-specific files (??.sln, .vcxproj)
- Large test images (use placeholder URLs)
- Copyrighted content without permission
- Breaking changes without discussion

## ?? Communication

- **Issues**: For bug reports and feature requests
- **Pull Requests**: For code contributions
- **Discussions**: For questions and ideas
- **Email**: For private matters

## ?? License

By contributing, you agree that your contributions will be licensed under the MIT License.

## ?? Recognition

Contributors will be:
- Listed in CONTRIBUTORS.md
- Credited in release notes
- Mentioned in README.md (major contributions)

---

Thank you for contributing to Naghuma Toolbox! Your efforts help make image processing accessible to everyone. ??
