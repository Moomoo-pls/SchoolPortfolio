/*
  This is a little adventure game.  There are three
  entities: you, a treasure, and an ogre.  There are 
  six places: a valley, a path, a cliff, a fork, a maze, 
  and a mountaintop.  Your goal is to get the treasure
  without being killed first.
*/

/*
  First, text descriptions of all the places in 
  the game.
*/
description(valley,
  'You are in a pleasant valley, with a trail ahead.').
description(path,
  'You are on a path, with ravines on both sides.').
description(cliff,
  'You are teetering on the edge of a cliff.').
description(fork,
  'You are at a fork in the path.').
description(maze(_),
  'You are in a maze of twisty trails, all alike.').
description(mountaintop,
  'You are on the mountaintop.').
description(labyrinth(_),
  'You are lost in a winding corridor.').

/*
  report prints the description of your current
  location.
*/
report :-
  at(you,X),
  description(X,Y),
  write(Y), nl.

/*
  These connect predicates establish the map.
  The meaning of connect(X,Dir,Y) is that if you
  are at X and you move in direction Dir, you
  get to Y.  Recognized directions are
  forward, right, and left.
*/
connect(valley,forward,path).
connect(path,right,cliff).
connect(path,left,cliff).
connect(path,forward,fork).
connect(fork,forward,labyrinth(0)).
connect(fork,left,maze(0)).
connect(fork,right,mountaintop).
connect(mountaintop,left,fork).
connect(maze(0),left,maze(1)).
connect(maze(0),right,maze(3)).
connect(maze(1),left,maze(0)).
connect(maze(1),right,maze(2)).
connect(maze(2),forward,maze(4)).
connect(maze(2),left,fork).
connect(maze(2),right,maze(0)).
connect(maze(3),left,maze(0)).
connect(maze(3),right,maze(3)).
connect(maze(4),forward,mountaintop).
connect(maze(4),right,maze(4)).
connect(labyrinth(0),left,labyrinth(1)).
connect(labyrinth(0),right,labyrinth(15)).
connect(labyrinth(1),left,labyrinth(2)).
connect(labyrinth(1),right,labyrinth(10)).
connect(labyrinth(2),left,labyrinth(3)).
connect(labyrinth(2),right,labyrinth(9)).
connect(labyrinth(3),right,labyrinth(4)).
connect(labyrinth(4),left,labyrinth(5)).
connect(labyrinth(4),right,labyrinth(9)).
connect(labyrinth(5),left,labyrinth(6)).
connect(labyrinth(5),right,labyrinth(8)).
connect(labyrinth(6),right,labyrinth(7)).
connect(labyrinth(7),left,labyrinth(8)).
connect(labyrinth(7),right,labyrinth(12)).
connect(labyrinth(8),left,labyrinth(7)).
connect(labyrinth(8),right,labyrinth(9)).
connect(labyrinth(9),left,labyrinth(10)).
connect(labyrinth(9),right,labyrinth(8)).
connect(labyrinth(10),left,labyrinth(9)).
connect(labyrinth(10),right,labyrinth(11)).
connect(labyrinth(11),left,labyrinth(12)).
connect(labyrinth(11),right,labyrinth(10)).
connect(labyrinth(12),left,labyrinth(11)).
connect(labyrinth(12),right,labyrinth(13)).
connect(labyrinth(13),left,labyrinth(14)).
connect(labyrinth(14),left,labyrinth(15)).
connect(labyrinth(14),right,labyrinth(11)).
connect(labyrinth(15),left,fork).
connect(labyrinth(15),right,fork).

/*
  This sets all the configurations
*/
config(path, '0').
config(cliff, '1').
config(fork, '2').
config(mountaintop, '3').
config(maze(0), '4').
config(maze(1), '5').
config(maze(2), '6').
config(maze(3), '7').
config(maze(4), '8').
config(labyrinth(0), '9').
config(labyrinth(1), '10').
config(labyrinth(2), '11').
config(labyrinth(3), '12').
config(labyrinth(4), '13').
config(labyrinth(5), '14').
config(labyrinth(6), '15').
config(labyrinth(7), '16').
config(labyrinth(8), '17').
config(labyrinth(9), '18').
config(labyrinth(10), '19').
config(labyrinth(11), '20').
config(labyrinth(12), '21').
config(labyrinth(13), '22').
config(labyrinth(14), '23').
config(labyrinth(15), '24').

/*
  move(Dir) moves you in direction Dir, then
  prints the description of your new location.
*/
move(Dir) :-
  at(you,Loc),
  connect(Loc,Dir,Next),
  retract(at(you,Loc)),
  assert(at(you,Next)),
  report,
  !.
/*
  But if the argument was not a legal direction,
  print an error message and don't move.
*/
move(_) :-
  write('That is not a legal move.\n'),
  report.

/*
  Shorthand for moves.
*/
forward :- move(forward).
left :- move(left).
right :- move(right).

/*
  If you and the ogre are at the same place, it 
  kills you.
*/
ogre :-
  at(ogre,Loc),
  at(you,Loc),
  write('An ogre sucks your brain out through\n'),
  write('your eye sockets, and you die.\n'),
  retract(at(you,Loc)),
  assert(at(you,done)),
  !.
/*
  But if you and the ogre are not in the same place,
  nothing happens.
*/
ogre.

/*
  If you and the treasure are at the same place, you
  win.
*/
treasure :-
  at(treasure,Loc),
  at(you,Loc),
  write('There is a treasure here.\n'),
  write('Congratulations, you win!\n'),
  retract(at(you,Loc)),
  assert(at(you,done)),
  !.
/*
  But if you and the treasure are not in the same
  place, nothing happens.
*/
treasure.

/*
  If you are at the cliff, you fall off and die.
*/
cliff :-
  at(you,cliff),
  write('You fall off and die.\n'),
  retract(at(you,cliff)),
  assert(at(you,done)),
  !.
/*
  But if you are not at the cliff nothing happens.
*/
cliff.

/*
	Read the configuration, and returns the needed numbers.
*/
read_config(Concodes, Con1, Con2) :-
  string_to_list(Conline, Concodes),
  atomic_list_concat(L, ',', Conline),
  nth0(0, L, Con1),
  nth0(1, L, Con2).

/*
  Game loop.  Stop if player won or lost.
*/
game(Infile, []) :- 
  at(you,done),
  !.

/*
  Game loop.  Not done, so get a move from the user
  and make it.  Then run all our special behaviors.  
  Then repeat.
*/
game(Infile, [Move|Next]) :-
  \+ at(you,done),
  read(Infile, Move),
  move(Move),
  ogre,
  treasure,
  cliff,
  game(Infile, Next).

/*
	Main loop. Ends once all scenarios are run
*/

main(Confile, []) :-
	at_end_of_stream(Confile),
	!.

/*
  Main loop. Runs all scenarios.
*/

main(Confile, [Config|Next]) :-
	\+ at_end_of_stream(Confile),
	retractall(at(_,_)),
	assert(at(you,valley)),
	read_line_to_codes(Confile, Config),
	read_config(Config, Onum, Tnum),
	config(Oloc, Onum),
	config(Tloc, Tnum),
	assert(at(ogre, Oloc)),
	assert(at(treasure, Tloc)),
	open('commands_v1',read,Ins),
	game(Ins, Commands),
	main(Confile, Next).

/*
  This is the starting point for the game.  We
  assert the initial conditions, print an initial
  report, then start the main loop.
*/
go :-

  open('configurations.txt',read,Str),
  main(Str, Configurations).
