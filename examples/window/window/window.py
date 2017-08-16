import naga

class ExampleGame(naga.Game):
    def on_run_start(self):
        naga.platform.window_size = naga.Vec2I(1280, 720)
        naga.states.push(ExampleState(), naga.STATE_FLAG_ALL)

    def on_run_end(self):
        pass

    def on_input_event(self, e):
        if e.device_type == naga.InputEvent.KEYBOARD and e.keyboard.type == naga.InputEvent.Keyboard.Type.KEY_PRESS and e.keyboard.key == naga.InputEvent.Keyboard.Key.ESCAPE:
            naga.app.exit()

    def on_window_event(self, e):
        pass


class PskComponent(naga.GameComponent):
    def __init__(self):
        self.psk = naga.PSK('C:\Users\Colin\Desktop\m3\m16_body.psk')
        return super(PskComponent, self).__init__()

    def on_render(self, camera_params):
        self.psk.render(camera_params)


class FreeLookComponent(naga.GameComponent):
    def __init__(self):
        super(FreeLookComponent, self).__init__()
        self.pitch = 0.0
        self.yaw = 0.0
        self.speed = 100.0
        self.local_velocity_target = naga.Vec3F(0, 0, 0)
        self.local_velocity = naga.Vec3F(0, 0, 0)
        self.sensivity = 0.125
        self.pitch_min = -89.0
        self.pitch_max = 89.0

    def on_input_event(self, e):
        super(FreeLookComponent, self).on_input_event_base(e)
        if e.device_type == naga.InputEvent.TOUCH:
            if e.touch.type == naga.InputEvent.Touch.Type.MOVE:
                self.yaw += e.touch.location_delta.x * self.sensivity
                self.pitch += e.touch.location_delta.y * self.sensivity
                # clamp pitch
                self.pitch = min(max(self.pitch, self.pitch_min), self.pitch_max)
        elif e.device_type == naga.InputEvent.KEYBOARD:
            if e.keyboard.key == naga.InputEvent.Keyboard.Key.W:
                if e.keyboard.type == naga.InputEvent.Keyboard.Type.KEY_PRESS:
                    self.local_velocity_target.z = 1.0
                elif e.keyboard.type == naga.InputEvent.Keyboard.Type.KEY_RELEASE:
                    self.local_velocity_target.z = 0.0
            elif e.keyboard.key == naga.InputEvent.Keyboard.Key.S:
                if e.keyboard.type == naga.InputEvent.Keyboard.Type.KEY_PRESS:
                    self.local_velocity_target.z = -1.0
                elif e.keyboard.type == naga.InputEvent.Keyboard.Type.KEY_RELEASE:
                    self.local_velocity_target.z = 0.0
            if e.keyboard.key == naga.InputEvent.Keyboard.Key.A:
                if e.keyboard.type == naga.InputEvent.Keyboard.Type.KEY_PRESS:
                    self.local_velocity_target.x = 1.0
                elif e.keyboard.type == naga.InputEvent.Keyboard.Type.KEY_RELEASE:
                    self.local_velocity_target.x = 0.0
            elif e.keyboard.key == naga.InputEvent.Keyboard.Key.D:
                if e.keyboard.type == naga.InputEvent.Keyboard.Type.KEY_PRESS:
                    self.local_velocity_target.x = -1.0
                elif e.keyboard.type == naga.InputEvent.Keyboard.Type.KEY_RELEASE:
                    self.local_velocity_target.x = 0.0


    def on_tick(self, dt):
        strength = 8.0 # TODO: move elsewhere
        self.local_velocity += (self.local_velocity_target - self.local_velocity) * dt * strength
        # yaw
        yaw = naga.angle_axis(self.yaw, naga.Vec3F(0, 1, 0))
        # pitch
        pitch = naga.angle_axis(self.pitch, naga.Vec3F(1, 0, 0))
        rotation = pitch * yaw
        self.owner.pose.rotation = rotation
        delta = (naga.inverse(rotation) * (self.local_velocity * self.speed)) * dt
        self.owner.pose.location = self.owner.pose.location + delta


class ExampleState(naga.State):
    def on_enter(self):
        # TODO: this should be done by the underlying state logic!
        bounds = naga.AABB2F()
        bounds.max.x = naga.platform.screen_size.x
        bounds.max.y = naga.platform.screen_size.y
        self.layout.bounds = bounds

        naga.platform.is_cursor_centered = True
        naga.platform.is_cursor_hidden = True

        self.scene = naga.Scene()
        self.camera = self.scene.create_game_object()
        camera_component = self.camera.add_component(naga.CameraComponent)
        free_look_component = self.camera.add_component(FreeLookComponent)

        self.terrain = self.scene.create_game_object()
        terrain_component = self.terrain.add_component(naga.TerrainComponent)
        terrain_component.set_heightmap(naga.resources.get_image('heightmap.png'))
        terrain_component.scale = naga.Vec3F(1.0, 1.0, 1.0)

        self.canvas = naga.GuiCanvas()
        self.canvas.dock_mode = naga.GuiDockMode.FILL
        self.layout.adopt(self.canvas)

        self.label = naga.GuiLabel()
        self.label.bitmap_font = naga.resources.get_bitmap_font('Fantasque Sans Mono.fnt')
        self.label.dock_mode = naga.GuiDockMode.FILL
        self.label.is_multiline = True
        self.layout.adopt(self.label)

        self.model = self.scene.create_game_object()
        model_component = self.model.add_component(naga.ModelComponent)
        model_component.model = naga.resources.get_model('boblampclean.md5m')
        model_component.play('boblampclean.md5a')
        self.model.add_component(PskComponent)

    def on_input_event(self, e):
        super(ExampleState, self).on_input_event_base(e)
        if e.device_type == naga.InputEvent.TOUCH:
            if e.touch.type == naga.InputEvent.Touch.Type.PRESS:
                camera_component = self.camera.get_component(naga.CameraComponent)
                # TODO: this is cumbersome!
                viewport = naga.RectangleI()
                viewport.width = naga.platform.screen_size.x
                viewport.height = naga.platform.screen_size.y
                ray = camera_component.get_ray(viewport, naga.Vec2F(naga.platform.cursor_location.x, naga.platform.cursor_location.y))
                terrain_component = self.terrain.get_component(naga.TerrainComponent)
                if terrain_component is not None:
                    hit_location = terrain_component.trace(ray)
                    print hit_location
                    self.model.pose.location = hit_location
        self.scene.on_input_event(e)

    def render(self):
        self.scene.render(self.canvas.frame_buffer, self.camera)
        super(ExampleState, self).render_base()

    # TODO: not sure why this is necessary?
    def on_tick(self, dt):
        self.label.string = '\n'.join([naga.gpu.vendor, naga.gpu.renderer, naga.gpu.version, str(naga.app.performance.fps)])
        self.scene.tick(dt)


naga.app.run(ExampleGame())