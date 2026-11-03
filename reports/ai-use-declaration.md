# AI-Use Declaration

**Project:** vizlib, Group 7
**Covers:** Week 1 (28 September) to the final submission (5 November 2026)

This declaration collects what each member declared in the weekly reports in `reports/`, plus the AI work done on the shared code during integration. Each member must check their own entry before submission and add anything that is missing, because undeclared AI use is penalised.

## By member

### Member 1: Kizito Mark James (data loading)
- **Tool:** Gemini
- **Purpose:** Research on CSV parsing and data structures (Week 1). Header design with `std::optional` and the structure of the research file and reports (Weeks 2 to 5).
- **Reason:** To speed up research and header design.
- **Note:** The same text was declared in Weeks 3, 4 and 5. James should state what AI was used for in the full data loading module (#108) and in his Week 3 to 5 reports.

### Member 2: Natukunda Melissa (statistics)
- **Tools:** Gemini, Claude
- **Purpose:**
  - Week 1: research on the statistics module (Gemini).
  - Week 2: the header skeleton (Claude) and help with the mode function (Gemini).
  - Week 3: understanding encapsulation in `summaryStatistics` (Claude).
  - She also used Wikipedia to understand the Freedman–Diaconis rule. That is not an AI tool.
- **Reason:** To understand the topics and the OOP design.
- **Note:** No AI use is declared for the KDE (#107), because she has no Week 4 or 5 report yet.

### Member 3: Zawedde Crystal (ECDF and rug plots)
- **Tool:** Gemini
- **Purpose:** Debugging C++ syntax and type-casting errors, using `std::sort`, and safe casting with `static_cast<double>` (Week 3).
- **Reason:** To write correct, standard C++.
- **Note:** She declared no AI use in Week 1, and has no Week 4 or 5 report yet.

### Member 4: Kiwalabye Oscar Muwanguzi (scatter, histogram, joint plot; repo owner)
- **Tool:** Claude (claude.ai and Claude Code)
- **Purpose, Week 1:**
  - reviewing the task split and the repo workflow
  - starting configuration for CMake, doctest and GitHub Actions
  - the report and changelog templates
  - explaining joint plots, binning and KDE, and drafting the joint plots research file
- **Purpose, Weeks 2 to 5:** designing and drafting the scatter plot, histogram, `Plot` hierarchy, joint plot, templates, `Numeric` concept and KDE joint plot, with their tests and examples. Running the tests and sanitizers, and drafting his weekly reports.
- **Purpose, integration (Weeks 3 to 5):** Claude Code was used to rebuild team pull requests that conflicted with `main`. It also wrote the fixes in those pull requests:
  - Fixed `kde.hpp` and wrote `tests/test_kde.cpp` (Member 2's module, #107).
  - Converted Member 5's test programs to doctest (#106).
  - Added missing files to CMake (#90, #107, #108).
  - Combined Member 3's ECDF pull requests (#90).
  - Fixed the build after merges (#71).
- **Purpose, 3 November:** Claude Code built a missing piece of Member 3's module: `ECDFPlot` drawing as a `Plot`, and `RugPlot`, with their tests and the drawn part of `ecdf_example.cpp` (#122). It also added `Figure::drawAxes` and tests for Member 5's `drawAxes`, and drafted the Week 3 to 5 group reports, this declaration, the final report, the presentation guide and the UML diagram.
- **Reason:** To finish his module despite missing dependencies, and to get the whole team's work integrated, tested and building on Linux, Windows and macOS before the deadline.

### Member 5: Mugabe Destiny (axis scaling and layout)
- **Tool:** ChatGPT (Weeks 1 and 2), and an AI assistant she did not name (Week 3)
- **Purpose:** Understanding axis scaling, tick generation and CMake, organising her research (Weeks 1 and 2). Understanding header and source organisation, the maths of tick generation, and OOP concepts (Week 3).
- **Reason:** To learn the topic before and while coding.
- **Note:** There is no AI section in her Week 4 report, and no Week 5 report yet. Her test files were converted to doctest by Member 4 using Claude Code (#106).

### Member 6: Nambalirwa Flavia Cate (Canvas rendering)
- **Tools:** Claude, Gemini, ChatGPT
- **Purpose:**
  - Week 1: explaining Canvas drawing algorithms and tidying her research (Claude), and research (Gemini).
  - Week 2: explanations and reviewing her headers (Claude).
  - Week 3: understanding and improving parts of her code, and organising her report (ChatGPT and Claude).
  - Week 4: understanding clipping, input validation and tests, and drafting code changes and test cases, which she reviewed, entered and tested herself (Claude). Reading build output, Git branch and stash steps, and improving her report (ChatGPT).
- **Reason:** To understand the algorithms and write safer code and clearer reports.

### Member 7: Namaganda Norah Margret (image export, build, CI, pipeline)
- **Tools:** ChatGPT, Claude
- **Purpose:**
  - Week 1: understanding CMake, CI and stb_image_write (ChatGPT).
  - Week 2: guidance and drafting (Claude).
  - Week 4: preparing her report (ChatGPT).
  - Week 5: her report was prepared with help from AI tools she did not name.
- **Reason:** To learn the build tools and write the reports.
- **Note:** There is no AI section in her Week 3 report.

### Member 8: Kabahamba Joy A Maria (text, labels, legends, annotations)
- **Tool:** ChatGPT (Weeks 1 and 2), and an AI assistant she did not name (Week 3)
- **Purpose:** Understanding stb_truetype and text layout, and structuring her module (Weeks 1 and 2). Understanding build errors and failed CI checks (Week 3).
- **Reason:** To learn the topic and plan the work.
- **Note:** There is no AI section in her Week 4 or Week 5 report.

## Group statement

AI tools were used as support. They explained topics, drafted code and reports for review, and helped find and fix build problems. Every member is responsible for understanding and explaining the code in their module, including any part that was drafted with AI or written for them by another member, as noted above.
