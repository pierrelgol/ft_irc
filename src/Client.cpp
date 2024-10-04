/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 14:25:41 by pollivie          #+#    #+#             */
/*   Updated: 2024/10/04 14:25:42 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool client_append_to_internal_buffer (Client_t *const client, const char *const buffer) {
        if (!client or !buffer) {
                return (false);
        }

        if (!client->buffer) {
                client->buffer = strdup (buffer);
                if (!client->buffer) {
                        return (false);
                }
        } else {
                isize old_buffer_size = strlen (client->buffer);
                isize new_buffer_size = strlen (buffer);
                isize total_size      = old_buffer_size + new_buffer_size;

                char *new_buffer = (char *) calloc (total_size + 1, sizeof (char));
                if (!new_buffer) {
                        return (false);
                }

                strncat (new_buffer, client->buffer, total_size);
                strncat (new_buffer, buffer, total_size);
                free (client->buffer);
                client->buffer = new_buffer;
        }
        return (true);
}

const char *client_get_internal_buffer (Client_t *const client) {
        return (client->buffer);
}

bool client_disconnect (Client_t *client) {
        if (!client) {
                return (false);
        } else {
                log_info ("Client is disconnecting!");
                client_clear_internal_buffer (client);
                close (client->fd);
        }
        return (true);
}

bool client_clear_internal_buffer (Client_t *client) {
        if (!client or !client->buffer) {
                return (false);
        }
        free (client->buffer);
        client->buffer = NULL;
        return (0);
}
