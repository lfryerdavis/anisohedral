# anisohedral
This is a project to find anisohedral criteria for polyforms.

## Background

## Software Guide
The simplest set of options is to see the boundaries of all the shapes that are going to be used in the project.

```
./anisohedral -laves -tests -boundaryonly -o boundaries.tex -showneighbours -standalone
```
The `-o` option is used to specify the output. The default name is `out.tex`. In this case, the file is called `boundaries.tex`.

The `-laves` option shows a representative of all Laves tiles. These tiles can also be generated using single options such as `-hex`, `-floretleft`, and so on. The different options are found in the `anisohedral.cpp` file.
<img width="715" height="399" alt="laves1" src="https://github.com/user-attachments/assets/758bd47e-22b7-4881-bfb8-c9e04ffc89df" />
<img width="722" height="449" alt="laves2" src="https://github.com/user-attachments/assets/19f48a1d-097a-469b-b587-b7b9c48436de" />
<img width="720" height="367" alt="laves3" src="https://github.com/user-attachments/assets/e81d01aa-dfd6-4fd5-87c4-0fff24a11c98" />
<img width="734" height="356" alt="laves4" src="https://github.com/user-attachments/assets/705f1d52-f135-4fba-8642-267227006deb" />
<img width="724" height="382" alt="laves5" src="https://github.com/user-attachments/assets/0ffff41d-32dc-48cb-bbde-c2bad20a1020" />
<img width="717" height="321" alt="laves6" src="https://github.com/user-attachments/assets/5eb7d4bc-6478-4554-9d95-a2feffdaeb37" />

The `-tests` option shows a representative of all the test tiles. These tiles can also be generated using single options. The test anchor tiles all have a different placement of knots (identified by blue circles instead of black circles) to ensure that the anchor placement code is working correctly. The different options are found in the `anisohedral.cpp` file.
<img width="703" height="291" alt="test1" src="https://github.com/user-attachments/assets/97a63491-028a-42b1-a870-af4714e31489" />
<img width="705" height="287" alt="test2" src="https://github.com/user-attachments/assets/7a465381-3fec-4e27-84df-d80c8284e189" />
<img width="705" height="287" alt="test2" src="https://github.com/user-attachments/assets/cde1da00-ff0c-4b34-abce-89a80b64954c" />
<img width="719" height="225" alt="test4" src="https://github.com/user-attachments/assets/39fa5269-aaf1-4e86-8b71-7f1844b1b359" />
<img width="719" height="183" alt="test5" src="https://github.com/user-attachments/assets/ddc407bf-f821-4da2-a813-275e0d06c4c8" />

The `-boundaryonly` option will only show the boundary of the generated tiles. There will be no tile splits.

For the Laves tiles, the tiles can have the neighbours generated using the -showneighbours option. This will be used to count the vertex degree of each tile using the boundaries when extracting the tile topologies.

The `-standalone` option is used to have a standalone TikZ file instead of a regular LaTeX file. This will probably be what you want, to allow the copy and paste of the standalone TikZ code into a LaTeX file, or to copy the compiled picture into other files. The regular LaTeX file will not be scaled, so the output will often not fit into a page.

