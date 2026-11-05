# Design Decisions

## Why the Core Rendering Classes Are Not Templates

The core rendering classes in Vizlib, such as `Figure`, `Axes`, `Canvas`, `TextRenderer`, `Annotations`, and `Legend`, are implemented as concrete classes rather than class templates.

This is an intentional design choice for the following reasons:

### 1. Consistent data types

The rendering system uses established types such as `double` for coordinates and measurements, `Point` and `Rect` for geometry, and `Color` for colours. These types provide a consistent representation throughout the library.

### 2. Simpler interfaces

Concrete classes provide straightforward interfaces that are easier to use, understand, test, and maintain. Users do not need to specify template arguments when creating figures, axes, annotations, or legends.

### 3. Shared rendering components

The rendering classes work together through common interfaces. For example, `Drawable` defines a shared drawing interface, while `Canvas` and `TextRenderer` provide the functionality needed to render visual elements.

Using fixed types allows these components to work together without introducing unnecessary template complexity.

### 4. No current need for multiple type variations

The core rendering functionality does not currently require separate implementations for different coordinate, colour, or canvas types. Templates would add complexity without providing a significant benefit for the current requirements.

### Conclusion

The core rendering classes are concrete because Vizlib currently uses a consistent set of numerical, geometric, and rendering types. This keeps the public API simple and makes the components easier to integrate and maintain.

This decision does not prevent templates from being used in other parts of Vizlib where generic programming is useful.
