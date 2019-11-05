# Type Driven User Interface with Qt and C++
Presentation for the [Qt World Summit 2019](https://www.qt.io/qtws19) (4.-6. November 2019 in Berlin)

## Slides

You can view slides at https://arbmind.github.io/2019-types-en

## Code

The `code/` folder contains a compilable version of all the code examples from the slides.

You need a C++17 capable compiler. QBS is used as project management, but it should be easy to setup with any build system.

*Warning:* All the code is for learning and demonstration purposes only and has no production quality.
Especially the slide code is aimed to fit on slides and lacks a lot of best practices.

## Slide Tech

* [AsciiDoctor](https://github.com/asciidoctor/asciidoctor) as hackable html slide generator with a lot of features
* [Bespoke.js](https://github.com/bespokejs/bespoke) as a flexible html slide framework
* [Patched AsciiDoctor-Bespoke](https://github.com/arBmind/asciidoctor-bespoke/tree/patch-1) that allows the nested code display
* [Patched Bespoke-Bullets](https://github.com/arBmind/bespoke-bullets) to allow better interactions through API
* [Patched Bespoke-Hash](https://github.com/arBmind/bespoke-hash) that uses the extended bullets API
* [Patched Bespoke-OnStage](https://github.com/arBmind/bespoke-onstage) with some customizations and fixes for my setup
* [Gulp](https://github.com/gulpjs/gulp) to automate regeneration and push slide updates to the browser

## License

The slides here are available under the terms of the Creative Commons Attribution-ShareAlike license.
[(cc-by-sa-license)](https://creativecommons.org/licenses/by-sa/2.0/)

Photos

* [Grand Valley cattle drive](https://www.flickr.com/photos/stormfarm/14704893215) [(cc-by-sa-license)](https://creativecommons.org/licenses/by-sa/2.0/) - Cropped & Auto Toning
