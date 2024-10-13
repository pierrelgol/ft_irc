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

        // if there is no client or buffer we early return
        if (!client or !buffer) {
                return (false);
        }

        // if there is a client but it doesn't have a buffer we can just strdup the new buffer
        if (!client->buffer) {
                client->buffer = strdup (buffer);
                if (!client->buffer) {
                        return (false);
                }
        } else {
                // of we will manually join the buffer togethers
                isize old_buffer_size = strlen (client->buffer);
                isize new_buffer_size = strlen (buffer);
                isize total_size      = old_buffer_size + new_buffer_size;

                // it's important to use calloc here for proper concactenation to happen
                char *new_buffer = (char *) calloc (total_size + 1, sizeof (char));
                if (!new_buffer) {
                        // if the allocation fails for now we just return but there's probably a better way to do it
                        // has this means we are effectively loosing our client's data, and now that I'm writting
                        // this I think it might be wiser to even free the current client's buffer because the command
                        // we would parse otherwise would be erronous because they would be incomplete
                        // @TODO figure out whether to free or not the current buffer
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

// this function will log the disconnection
// free the internal buffer of the client
// close it's file descriptor
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

// this function will free the client's internal buffer
// and set it back to null
bool client_clear_internal_buffer (Client_t *client) {
        if (!client or !client->buffer) {
                return (false);
        }
        free (client->buffer);
        client->buffer = NULL;
        return (true);
}
