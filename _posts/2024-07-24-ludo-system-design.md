---
title: Ludo System Design
time: 2024-07-24 19:41:46
categories: [projects, ludo]
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

1. Scope: Build Ludo game that can be play by a single user on browser (Can be a single browser for now)
1. Objectives: Allow single

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
