from mandala import *


class PauseState(State):
	def __init__(self):
		State.__init__(self)

	def tick(self, dt):
		print dt

s = PauseState()

states.push(s, STATE_FLAG_ALL)
states.pop(s)

print states.index_of(s)