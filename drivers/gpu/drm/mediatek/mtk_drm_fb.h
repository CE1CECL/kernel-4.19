
#ifndef MTK_DRM_FB_H
#define MTK_DRM_FB_H

int mtk_fb_wait(struct drm_framebuffer *fb);
struct drm_framebuffer *mtk_drm_mode_fb_create(struct drm_device *dev,
					       struct drm_file *file,
					       const struct drm_mode_fb_cmd2 *cmd);

#endif /* MTK_DRM_FB_H */
