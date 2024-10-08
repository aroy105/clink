# Clink :musical_note:

## A _fast_ and _customizable_ Mancala State Simulator built by Arghya Roy

Does your family yell at you when you take forever strategizing about Mancala? :speak_no_evil:

Do you find it hard to remember your pre-calculated moves when playing against your significant other? :anguished:

Are you frusturated by the cumbersome stones, the shallow pits, and the snail pace of physical Mancala? :zzz:

Sounds like YOU :ok_woman: need Clink in your life!

Clink is a simple Manacla State simulator that lets you plan and rapidly decide what line you want to play, independent of what game mode you want to play. 

Currently, three game modes exist: Capture, Avalanche, and Capture-Avalanche. The rules for Capture and Avalanche are based off GamePigeon rules, and for Capture-Avalanche, it's based off what the creator remembers the rules are from previous conversations and what makes sense. If the rules need to be modified, feel free to reach out to the creator. 

![What you should see when you run the project](./default-view.jpeg)

## Installation
1. Download this project, **OR** if you have Git, from the terminal run:
```
git clone https://github.com/aroy105/clink.git
```
2. If need be, download [Docker](https://www.docker.com/), and keep Docker on while you use Clink
3. Go back to your terminal, and navigate into the project directory
4. From the terminal, enter in the following command:
```
chmod +x run.sh
```
5. Then, enter in:
```
./run.sh
```
6. The first time you do this, it'll probably take some time as some of the dependencies are being downloaded. Once all the downloads have finished and the terminal has finished running all the commands, go to your web browser, and enter this in as the url:
```
http://localhost:8080
```
7. Once you are done playing, go back to your terminal, and control + C to kill the program. Then, open up Docker, and delete whatever container (which will have two random names slapped together) was created. 

If you want to run this again in the future, just open Docker, navigate to the directory, and run
```
./run.sh
```









Dedicated to ARY