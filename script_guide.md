Open a Ubuntu terminal and Docker.
Insure your Ubuntu can run docker
run following commands:
`cd jajapy`
`gg-summer.registry.jetbrains.space/p/p7/jajapy/full:latest -f Dockerfile.full .`
`docker run -it gg-summer.registry.jetbrains.space/p/p7/jajapy/full:latest bash`
`python3 -m memray run <script_name>.py`
Do not close the image after the run is done. 

After the script is done, open a new terminal in another window.
Run the following command for finding the container id:
`docker ps`
Copy the results file and memray bin file from the container with this command:
`sudo docker cp <docker image id>:/base/jajapy/<file name> .`

Once that is done, we can make the binary files into HTML files for reading with this commando:
`python3 -m memray flamegraph <name of bin file>`