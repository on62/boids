/* SPDX-License-Identifier: MIT */
#ifndef __BOIDS_H__
#define __BOIDS_H__

#include <glib.h>
#include <glib/gprintf.h>

#include "vector.h"

#define DEFAULT_WIDTH  1024
#define DEFAULT_HEIGHT 576

#define DEFAULT_NUM_BOIDS 300
#define MIN_BOIDS 1
#define MAX_BOIDS 1000

#define DEFAULT_DEAD_ANGLE (60)

#define OBSTACLE_RADIUS 20

typedef struct {
	Vector pos;
	Vector velocity;
	gdouble red;

#ifdef BOIDS_DEBUG
	Vector avoid;
	Vector align;
	Vector cohesion;
	Vector obstacle;
#endif
} Boid;

typedef struct {
	Vector pos;
	guint flags;
} Obstacle;

#define OBSTACLE_FLAG_WALL 0x01

typedef struct _Swarm Swarm;

typedef void (*SwarmAnimateFunc)(Swarm *swarm, gulong time);

typedef struct _Swarm {
	GArray *boids;
	GArray *obstacles;

	gint width;
	gint height;

	gboolean walls;

	gboolean avoid;
	gboolean align;
	gboolean cohesion;
	gboolean dead_angle;
	gdouble cos_dead_angle;

	GThread  *move_th;
	gboolean move_th_running;
	SwarmAnimateFunc animate_cb;
	gpointer animate_cb_userdata;

#ifdef BOIDS_DEBUG
	gboolean debug_timing;
	gboolean debug_vectors;
#endif
} Swarm;

static inline gdouble deg2rad(gdouble deg)
{
	return deg * G_PI / 180;
}

static inline gdouble rad2deg(gdouble rad)
{
	return rad * 180 / G_PI;
}

#define POW2(v) ((v) * (v))

Swarm *swarm_alloc(guint num_boids, gboolean walls);
void swarm_free(Swarm *swarm);

void swarm_update_sizes(Swarm *swarm, guint width, guint height);

#define swarm_get_num_boids(swarm) ((swarm)->boids->len)
void swarm_set_num_boids(Swarm *swarm, guint num);

#define swarm_get_boid(swarm, n) (&g_array_index((swarm)->boids, Boid, n))

guint swarm_get_dead_angle(Swarm *swarm);
void swarm_set_dead_angle(Swarm *swarm, guint angle);

void swarm_add_obstacle(Swarm *swarm, gdouble x, gdouble y, guint flags);
gboolean swarm_remove_obstacle(Swarm *swarm, gdouble x, gdouble y);

#define swarm_get_obstacle(swarm, n) (&(g_array_index((swarm)->obstacles, Obstacle, n)))
#define swarm_get_obstacle_pos(swarm, n) (&(g_array_index((swarm)->obstacles, Obstacle, n).pos))
#define swarm_get_obstacle_flags(swarm, n) (g_array_index((swarm)->obstacles, Obstacle, n).flags)

void swarm_remove_walls(Swarm *swarm);
void swarm_add_walls(Swarm *swarm);

void swarm_thread_start(Swarm *swarm, SwarmAnimateFunc cb, gpointer userdata);
void swarm_thread_stop(Swarm *swarm);
gboolean swarm_thread_running(Swarm *swarm);

void swarm_move(Swarm *swarm);

int gtk_boids_run(Swarm *swarm);

#endif /* __BOIDS_H__ */
