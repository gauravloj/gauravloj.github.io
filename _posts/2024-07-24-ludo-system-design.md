---
title: Ludo System Design
time: 2024-07-24 19:41:46
categories: [Projects, Ludo]
tags: [system-design, architechture]
---

# Ludo WebApp Design discussions

This post talks about how the different design decisions were made in making the
web application to play a simple game of Ludo. Code for this webapp is hosted
at [Ludo WebApp Github](https://github.com/gauravloj/ludo-webapp).
It is designed and developed in collaboration with [Apoorva Ranade](https://github.com/apoorvaran)

## About the project

Goal of this project is to develop a webapp using Reactjs and nodejs and go through
all the development phases for real-life projects in a company. This is just to practice
our skills we gained throughout our career. So, we decided to build a simple game with rules
that are not too easy (like Tic-Tac-Toe) and not too complicated (like Chess). Here, rules
may be simple to implement but strategy to win can be complicated. Hence, Considering the
complexity of winning a game, we selected Ludo to be the best game to build with small set of
rules and contains medium level of complexity to win the game which will be discussed later,
maybe in another post.

To define in concrete terms:

1. Scope: Build Ludo game that can be played by a single user on browser (Can be a single browser for now)
1. Objectives: Allow User to play a two player game of Ludo against the bot.

## Defining Requirements

### Functional Requirements

Since this is a game, its basic requirement is to allow user to play a game. Other requirements
are narrowed down or ignored based on complexity of implementation or usefulness for the simplistic
state of the game. They are:

1. User should be able to play a game of Ludo with bot: This is limited to two player game for now for the following reason:

   - For 4 player game with 1 user and 3 bots, either all 3 bots play almost instantly which doesn't seem real or it increases
     the wait time for user for their next turn, both of them are not good user experience.
   - For 4 player game with multiple-user and less than 3 bots, it is good to play if the game is synchronized on multiple
     browsers. But since for initial release we only support game on single browser, it is inconvenient for more than 1 user
     to play the game on same device.

   This introduces future scope of the website to synchronize the game across multiple devices.

2. Ensure dice rolls are random: This focuses more on the pleasure in dealing with uncertainty in a game. Why?

   - If the dice rolls are predictable (example sequential), it makes the game easily winnable. And if it is easy to win,
     it is easy to move on to different game.
   - If it is Pseudo random (like repeating same pattern sequence of numbers every time), the interesting part is to find the
     pattern, but as soon as users finds the pattern it again becomes too easy to be interesting.
   - For a true random number sequence, since there is uncertainty that the user might lose, it forces user to pay attention
     to the game which keeps the user engaged with the game instead of blindly moving any piece and still win.

   This introduces the future scope of adding difficulty levels to the game.

3. User should be able to start a game any time: This is simply to give user the flexibility to decide the fate of the game.
4. User can resume the same game after closing the tab and opening it back again: This is to ensure that the game state
   is not lost by any accidental refresh of the page or to enable user to take breaks during the game.

### Non Functional Requirements

For a simple game, it doesn't need to be highly available of scalable at this point. One thing that is important
for a better user experience is to have a **good responsiveness** of the game, i.e. dice/pieces should react as soon as
the user clicks on them. Since it doesn't handle any sensitive information so security is also not required here.

## Design

### High Level Design

Since it is a single page web application without any need for scalability or availability,
we decided not to think about component diagram. There are only two components in action here,
i.e. user (browser) and Bot (requests are served from a single hosting server).

#### Sequence Diagram

This sequence diagram shows the user interaction with the website in an end to end flow.
It doesn't show the scenario when user goes to home page in the middle of game, or closes
the tab and opens it again.

![Sequence Diagram](assets/img/ludo/user-interaction-sequence-diagram.jpg)

#### Website Wireframe

Once the User interaction is decided, we created a basic look of the User Interface.
These wireframe diagram shows:

1. Landing page: Welcome the user, display game rules and let user pick their favorite color.
2. Game board: Selected color is always displayed as the bottom home box and all other colors are
   rotated accordingly in the same order.
3. Game outcome screen: Once the game is completed, it shows the status of the game and prompts user
   to start a new game.

![Landing page diagram](assets/img/ludo/ludo-wireframe-landing.jpg)
![Game board page diagram](assets/img/ludo/ludo-wireframe-gameboard.jpg)
![Game conclusion page diagram](assets/img/ludo/ludo-wireframe-conclusion.jpg)

### Game rules

1. Two player game
1. Game is played between user vs computer
1. Player can choose any color out of red, green, purple, yellow.
1. Player's home boxes will face each other
1. Every player will roll the dice alternatively
1. Once dice is rolled, user can select which piece to move
1. Player piece will move same steps as the number on the dice rolled.
1. Player whose all four pieces reaches the winning box first wins the game.
1. Game will end when any one of the player wins the game
1. User Will have to select the piece everytime a dice is rolled.

Future scope: User preference can be added to automatically move the piece if only single valid move

### Technology Stack and design decision

1. Reactjs for frontend: easy to make the game react on different events
1. Nodejs is used in backend to serve the app
1. All the game state will be stored in browser's local storage. It allows user to resume game any time they want.
1. Github is used for version control
1. Github Actions is used for continuous build and automated testing
1. Github issues can be used to track any tasks/issues for the website
1. For now we have not selected any cloud provider to host the website, but it can be decided once the website is fully developed.
