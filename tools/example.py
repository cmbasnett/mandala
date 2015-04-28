from mandala import *


class PauseState(State):
	def __init__(self):
		State.__init__(self)

for i in range(0, 1000):
	states.push(PauseState(), STATE_FLAG_ALL)