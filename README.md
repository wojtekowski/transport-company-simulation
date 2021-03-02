# transport-company-simulation
Simulation Techniques - Transport Company 

Abstract:
Presents the state-of-art techniques used in environment modelling, simulation-based evaluation of
discrete-event systems, clock advance mechanisms, activity scanning, random number generators and all
other examples of simulation models, methodology of computer simulation and source code
documentation with all strictly speaking.

Task:
• Simulate a transport company, where k trucks with capacity of u units transport cargo goods
between the headquarters and n regional depots.
• Goods arrive in batches of size r units, where r is a random variable following normal distribution
with mean µr and variance σr 2 (but the lowest possible size is 0.1 unit and the highest is 10 units).
• In the headquarters the cargo batch destination depot dn is a random variable following uniform
distribution with equal probability for each depot, while the cargo batch destination for all cargo
generated in depots is the headquarters.
• The time interval between the arrival (generation) of two consecutive cargo batches is a random
variable with exponential distribution and average chq and cd for the headquarters and each
depot, respectively.
• Cargo batches wait in queues at the headquarters and the depots until they are taken by the next
truck going to the destination point of a cargo batch, where the truck destination at the
headquarters is selected with strategy S, while for the depots the destination is always the
headquarters.
• After the destination is determined the cargo batches destined to this point are loaded in first-in
first-out order (FIFO) until the next one exceeds the truck capacity.
• Only full batch can be loaded, assuming there is enough space in the truck. In the headquarters
there are M loading/unloading platforms, while in each depot there are N loading/unloading
platforms, with single platform able to accommodate one truck.
• Trucks upon arrival enter a free platform unless all of them are occupied – in such a case they wait
in queue. Then they are unloaded and later loaded, with the loading and unloading time for each
cargo batch determined as r*Tl and r*Tu, respectively.
• Delivered cargo batches leave the system. Trucks leave the platform according to strategy P.
• The traveling time for each truck between any depot and the headquarters follows normal
distribution with average µt and variance σt 2.

<img width="321" alt="simulationtechniquesparameters" src="https://user-images.githubusercontent.com/33785730/109703934-56698980-7b96-11eb-9bc7-746794575b3d.PNG">

<img width="319" alt="simulationtechniquesmethods" src="https://user-images.githubusercontent.com/33785730/109703944-59fd1080-7b96-11eb-9225-fe881fcca36c.PNG">

<img width="258" alt="simulationtechniquesalgorithm" src="https://user-images.githubusercontent.com/33785730/109703967-5f5a5b00-7b96-11eb-8a47-e39e25f210a1.PNG">

<img width="370" alt="simulationtechniquesscheme" src="https://user-images.githubusercontent.com/33785730/109703988-641f0f00-7b96-11eb-893e-749a32df6055.PNG">

<img width="302" alt="simulationtechniquesoutput" src="https://user-images.githubusercontent.com/33785730/109704003-68e3c300-7b96-11eb-87de-a4481c1bb04f.PNG">

<img width="306" alt="simulationtechniquesinitial phase" src="https://user-images.githubusercontent.com/33785730/109704020-6b461d00-7b96-11eb-8d12-e34494d2b7cf.PNG">

The end, this project has received a very high final grading. Please do not reuse this code in your projects and plagiarism detectors can detect the algorithms and overview, or the teacher can simply Google the examples and catch you. Please study my code and learn how you can make your own object orientated simulation in C++. All the best. Wojciech.
