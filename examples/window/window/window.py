from naga import *

class ExampleGame(Game):
    def on_run_start(self):
        platform.window_size = Vec2(1280, 720)
        self.console = ConsoleState()
        states.push(ExampleState(), STATE_FLAG_ALL)

    def on_input_event(self, e):
        if e.type.device == InputDeviceType.KEYBOARD and e.type.action == InputActionType.PRESS:
            if e.type.key == Key.ESCAPE:
                app.exit()
            elif e.type.key == Key.F12:
                app.screenshot()
            elif e.type.key == Key.GRAVE_ACCENT:
                # toggle console
                if states.contains(self.console):
                    states.pop(self.console)
                else:
                    states.push(self.console, STATE_FLAG_RENDER | STATE_FLAG_TICK)
    
    def on_window_event(self, e):
        pass

class ConsoleState(State):

    def __init__(self):
        super(ConsoleState, self).__init__()

        self.root = GuiNode()
        self.root.anchor_flags = GUI_ANCHOR_FLAG_HORIZONTAL | GUI_ANCHOR_FLAG_TOP
        # TODO: we can fix this clunkiness by separating out the axes into different variables
        self.root.size_modes = GuiSizeModes(GuiSizeMode.RELATIVE, GuiSizeMode.RELATIVE)
        self.root.size = Vec2(1.0, 0.5)

        self.background = GUIImage()
        self.background.sprite = Sprite(resources.get_texture('white.png'))
        self.background.dock_mode = GuiDockMode.FILL
        self.background.color = Vec4(0.0, 0.0, 0.0, 0.5)

        self.input_label = GuiLabel()
        self.input_label.bitmap_font = resources.get_bitmap_font('Fantasque Sans Mono.fnt')
        self.input_label.is_read_only = False
        self.input_label.anchor_flags = GUI_ANCHOR_FLAG_BOTTOM
        self.input_label.size_modes = GuiSizeModes(GuiSizeMode.RELATIVE, GuiSizeMode.ABSOLUTE)
        self.input_label.size = Vec2(1.0, 32.0)
        self.input_label.should_clip = True
        self.input_label.on_enter_function = GuiLabel.on_enter_fn.from_callable(self.on_input_label_enter) # TODO: this syntax is a bit clunky, come up with something better!

        self.root.adopt(self.background)
        self.root.adopt(self.input_label)

        self.layout.adopt(self.root)

    def on_input_label_enter(self, label):
        try:
            py.execute(str(label.string))
        except Exception as e:
            print(e)
        label.string = ''
        return True


class FreeLookComponent(GameComponent):
    def __init__(self):
        super(FreeLookComponent, self).__init__()
        self.pitch = 0.0
        self.yaw = 0.0
        self.speed = 20.0
        self.local_velocity_target = Vec3(0, 0, 0)
        self.local_velocity = Vec3(0, 0, 0)
        self.sensitivity = 0.125
        self.pitch_min = -89.0
        self.pitch_max = 89.0

    def on_input_event(self, e):
        super(FreeLookComponent, self).on_input_event_base(e)
        if e.type.device == InputDeviceType.MOUSE:
            if e.type.action == InputActionType.MOVE:
                self.yaw += e.mouse.location_delta.x * self.sensitivity
                self.pitch += e.mouse.location_delta.y * self.sensitivity
                self.pitch = min(max(self.pitch, self.pitch_min), self.pitch_max)
        elif e.type.device == InputDeviceType.KEYBOARD:
            if e.type.key == Key.W:
                if e.type.action == InputActionType.PRESS:
                    self.local_velocity_target.z = 1.0
                elif e.type.action == InputActionType.RELEASE:
                    self.local_velocity_target.z = 0.0
            elif e.type.key == Key.S:
                if e.type.action == InputActionType.PRESS:
                    self.local_velocity_target.z = -1.0
                elif e.type.action == InputActionType.RELEASE:
                    self.local_velocity_target.z = 0.0
            if e.type.key == Key.A:
                if e.type.action == InputActionType.PRESS:
                    self.local_velocity_target.x = 1.0
                elif e.type.action == InputActionType.RELEASE:
                    self.local_velocity_target.x = 0.0
            elif e.type.key == Key.D:
                if e.type.action == InputActionType.PRESS:
                    self.local_velocity_target.x = -1.0
                elif e.type.action == InputActionType.RELEASE:
                    self.local_velocity_target.x = 0.0


    def on_tick(self, dt):
        strength = 8.0 # TODO: move elsewhere
        self.local_velocity += (self.local_velocity_target - self.local_velocity) * dt * strength
        # yaw
        yaw = angle_axis(self.yaw, Vec3(0, 1, 0))
        # pitch
        pitch = angle_axis(self.pitch, Vec3(1, 0, 0))
        rotation = pitch * yaw
        self.owner.pose.rotation = rotation
        delta = (inverse(rotation) * (self.local_velocity * self.speed)) * dt
        self.owner.pose.location = self.owner.pose.location + delta


class ExampleState(State):

    def __init__(self):
        super(ExampleState, self).__init__()
        self.is_tracing = False

    def on_enter(self):
        # TODO: this should be done by the underlying state logic!
        super(ExampleState, self).on_enter_base()

        platform.is_cursor_centered = True
        platform.is_cursor_hidden = True

        self.scene = Scene()
        self.camera = self.scene.create_game_object()
        camera_component = self.camera.add_component(CameraComponent)
        free_look_component = self.camera.add_component(FreeLookComponent)

        self.terrain = self.scene.create_game_object()
        terrain_component = self.terrain.add_component(TerrainComponent)
        terrain_component.set_heightmap(resources.get_image('heightmap.png'))
        terrain_component.scale = Vec3(1.0, 1.0, 1.0)

        self.canvas = GUICanvas()
        self.canvas.dock_mode = GuiDockMode.FILL
        self.layout.adopt(self.canvas)

        self.label = GuiLabel()
        self.label.bitmap_font = resources.get_bitmap_font('Fantasque Sans Mono.fnt')
        self.label.dock_mode = GuiDockMode.FILL
        self.label.is_multiline = True
        self.layout.adopt(self.label)

        self.model = self.scene.create_game_object()
        model_component = self.model.add_component(ModelComponent)
        model_component.model = resources.get_model('boblampclean.md5m')
        model_component.play('boblampclean.md5a')
        model_component.relative_pose.scale = Vec3(0.1, 0.1, 0.1)

    def on_input_event(self, e):
        super(ExampleState, self).on_input_event_base(e)
        if e.type.device == InputDeviceType.MOUSE:
            if e.type.action == InputActionType.PRESS:
                self.is_tracing = True
            if e.type.action == InputActionType.RELEASE:
                self.is_tracing = False
        self.scene.on_input_event(e)

    # TODO: would be nice to not have to specify this!
    def render(self):
        self.scene.render(self.canvas.frame_buffer, self.camera)
        super(ExampleState, self).render_base()

    def on_tick(self, dt):
        if self.is_tracing:
            ray = self.camera.get_component(CameraComponent).get_ray(platform.viewport, Vec2(platform.cursor_location.x, platform.cursor_location.y))
            trace_result = self.scene.trace(ray.start, ray.end)
            if trace_result.did_hit:
                self.model.pose.location = trace_result.location
        self.label.string = '\n'.join([gpu.vendor, gpu.renderer, gpu.version, str(int(app.performance.fps))])
        self.scene.tick(dt)


app.run(ExampleGame())