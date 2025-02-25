/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_calculation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelgharo <yelgharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 19:11:09 by asalek            #+#    #+#             */
/*   Updated: 2022/09/01 23:00:31 by yelgharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	dig_difftial_ans_paint(t_ray *ray, t_mlx *mlx, char **map, int x)
{
	double	camera_y_coordinate;

	ray->camera = 2 * x / (double)X_AXIS - 1;
	camera_y_coordinate = 2 * x / (double)Y_AXIS - 1;
	ray->raydirx = ray->dirx + ray->planex * ray->camera;
	ray->raydiry = ray->diry + ray->planey * ray->camera;
	ray->mapx = (int)ray->posx;
	ray->mapy = (int)ray->posy;
	if (ray->raydirx != 0)
		ray->deltadistx = fabs(1 / ray->raydirx);
	else
		ray->deltadistx = 7728;
	if (ray->raydiry != 0)
		ray->deltadisty = fabs(1 / ray->raydiry);
	else
		ray->deltadisty = 7728;
	calc_step_init_sidedist(ray, mlx, map);
}

void	calc_step_init_sidedist(t_ray *r, t_mlx *mlx, char **map)
{
	if (r->raydiry < 0)
	{
		r->stepy = -1;
		r->sidedisty = (r->posy - r->mapy) * r->deltadisty;
	}
	else
	{
		r->stepy = 1;
		r->sidedisty = (r->mapy + 1.0 - r->posy) * r->deltadisty;
	}
	if (r->raydirx < 0)
	{
		r->stepx = -1;
		r->sidedistx = (r->posx - r->mapx) * r->deltadistx;
	}
	else
	{
		r->stepx = 1;
		r->sidedistx = (r->mapx + 1.0 - r->posx) * r->deltadistx;
	}
	walls_hits(r, mlx, map);
}

void	walls_hits(t_ray *r, t_mlx *mlx, char **map)
{
	r->hit = 0;
	while (r->hit == 0)
	{
		r->img_n = 1;
		if (r->sidedistx < r->sidedisty)
		{
			r->sidedistx += r->deltadistx;
			r->mapx += r->stepx;
			r->side = 0;
			if (r->raydirx > 0)
				r->img_n = 0;
		}
		else
		{
			r->img_n = 3;
			r->sidedisty += r->deltadisty;
			r->mapy += r->stepy;
			r->side = 1;
			if (r->raydiry > 0)
				r->img_n = 2;
		}
		if (map[r->mapx][r->mapy] != '0')
			wall_hit_if(r, map);
	}
	wall_high(r, mlx);
}

void	wall_high(t_ray *r, t_mlx *mlx)
{
	if (r->side == 0)
		r->perpwalldist = r->sidedistx - r->deltadistx;
	else
		r->perpwalldist = r->sidedisty - r->deltadisty;
	r->line_height = (int)(Y_AXIS / r->perpwalldist);
	r->drawstart = -r->line_height / 2 + Y_AXIS / 2;
	if (r->drawstart < 0)
		r->drawstart = 0;
	r->drawend = r->line_height / 2 + Y_AXIS / 2;
	if (r->drawend >= Y_AXIS)
		r->drawend = Y_AXIS - 1;
	if (r->side == 0)
		r->wallx = r->posy + r->perpwalldist * r->raydiry;
	else
		r->wallx = r->posx + r->perpwalldist * r->raydirx;
	r->wallx -= floor(r->wallx);
	r->texx = (int)(r->wallx * (double)T_W);
	r->texx = T_W - r->texx - 1;
	r->steps = 1.0 * T_H / r->line_height;
	r->tex_position = (r->drawstart - Y_AXIS / 2 + \
		r->line_height / 2) * r->steps;
	walls_colors(r, mlx);
}

void	walls_colors(t_ray *r, t_mlx *mlx)
{
	int	i;

	i = 0;
	while (i < r->drawstart)
		r->tab[i++][mlx->x] = r->c_color;
	while (i < r->drawend)
	{
		r->texy = (int)r->tex_position;
		r->tex_position += r->steps;
		r->color = img_color(&r->img[r->img_n], r->texx, r->texy);
		r->tab[i][mlx->x] = r->color;
		i++;
	}
	while (i < Y_AXIS)
		r->tab[i++][mlx->x] = r->f_color;
}
