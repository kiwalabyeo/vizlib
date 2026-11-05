# Week 4 UML Class Diagram

This diagram shows the main classes involved in figure management, axes, annotations, legends, and text rendering in vizlib.

```mermaid
classDiagram
    class Figure {
        -int width_
        -int height_
        -deque~Axes~ axes_
        -string title_
        -string xLabel_
        -string yLabel_
        +addAxes()
        +getAxes()
        +getAxesCount()
        +setTitle()
        +getTitle()
        +setXLabel()
        +getXLabel()
        +setYLabel()
        +getYLabel()
        +drawLabels(Canvas, TextRenderer)
    }

    class Axes {
        -Rect plotArea_
        -LinearScale xLinear_
        -LinearScale yLinear_
        -LogScale xLog_
        -LogScale yLog_
        +setDataRange()
        +setPlotArea()
        +setXScale()
        +setYScale()
        +dataToPixel()
        +pixelToData()
        +getPlotArea()
        +drawTickLabels(Canvas, TextRenderer)
    }

    class Drawable {
        <<interface>>
        +draw(Canvas, TextRenderer)*
    }

    class Annotations {
        -vector~Annotation~ annotations_
        +add_text()
        +get_annotations()
        +draw(Canvas, TextRenderer)
        +clear()
    }

    class Annotation {
        +string text
        +Point position
        +double fontSize
        +Color color
    }

    class Legend {
        -vector~LegendEntry~ entries_
        -Point position_
        +add_entry()
        +get_entries()
        +clear()
        +set_position()
        +get_position()
        +draw(Canvas, TextRenderer)
    }

    class LegendEntry {
        +string label
        +Color color
    }

    class TextRenderer {
        -unique_ptr~Impl~ impl_
        +measure_text(string, double) TextMetrics
        +draw_text(Canvas, string, Point, double, Color)
    }

    class Canvas {
        -int width_
        -int height_
        -vector~Color~ pixels_
        +set_pixel()
        +get_pixel()
        +draw_line()
        +draw_rect()
        +fill_rect()
        +draw_circle()
        +fill_circle()
    }

    Figure "1" *-- "0..*" Axes : contains
    Annotations ..|> Drawable : implements
    Legend ..|> Drawable : implements
    Annotations "1" *-- "0..*" Annotation : contains
    Legend "1" *-- "0..*" LegendEntry : contains

    Figure ..> Canvas : uses
    Figure ..> TextRenderer : uses
    Axes ..> Canvas : uses
    Axes ..> TextRenderer : uses
    Annotations ..> Canvas : uses
    Annotations ..> TextRenderer : uses
    Legend ..> Canvas : uses
    Legend ..> TextRenderer : uses
    TextRenderer ..> Canvas : draws on
