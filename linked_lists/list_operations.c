#include "../includes/shell.h"

/**
 * add_node - Adds a node to the start of the list.
 * @head: Address of pointer to the head node.
 * @str: String field of the new node.
 * @num: Numeric field of the new node (e.g., node index for history).
 * Return: Pointer to the newly created head node, or NULL on failure.
 */
list_t *add_node(list_t **head, const char *str, int num)
{
	list_t *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(list_t));
	if (!new_head)
		return (NULL);

    // Initialize all members to 0/NULL (good practice)
	_memset((void *)new_head, 0, sizeof(list_t)); // Keep this line

	new_head->num = num;
	if (str)
	{
		new_head->str = _strdup(str); // This is correct.
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}
    // What if 'str' is NULL? new_head->str would remain NULL after _memset.
    // If you need it to be an empty string explicitly if str is NULL:
    else // if str is NULL, explicitly set new_head->str to a pointer to an empty string
    {
        new_head->str = _strdup(""); // Allocate memory for an empty string
        if (!new_head->str)
        {
            free(new_head);
            return (NULL);
        }
    }

	new_head->next = *head; // Link new node to the old head
	*head = new_head;       // Update head pointer to the new node
	return (new_head);
}

/**
 * add_node_end - Adds a node to the end of the list.
 * @head: Address of pointer to the head node.
 * @str: String field of the new node.
 * @num: Numeric field of the new node (e.g., node index for history).
 * Return: Pointer to the newly created node, or NULL on failure.
 */
list_t *add_node_end(list_t **head, const char *str, int num)
{
    list_t *new_node, *node;

    if (!head)
        return (NULL);

    node = *head;
    new_node = malloc(sizeof(list_t));
    if (!new_node)
        return (NULL);

    // Initialize all members to 0/NULL (good practice)
    _memset((void *)new_node, 0, sizeof(list_t)); // Keep this line

    new_node->num = num;
    if (str)
    {
        new_node->str = _strdup(str); // This is correct.
        if (!new_node->str)
        {
            free(new_node);
            return (NULL);
        }
    }
    // What if 'str' is NULL? new_node->str would remain NULL after _memset.
    // If you need it to be an empty string explicitly if str is NULL:
    else // if str is NULL, explicitly set new_node->str to a pointer to an empty string
    {
        new_node->str = _strdup(""); // Allocate memory for an empty string
        if (!new_node->str)
        {
            free(new_node);
            return (NULL);
        }
    }

    if (node) // If list is not empty, traverse to the end
    {
        while (node->next)
            node = node->next;
        node->next = new_node; // Link last node to the new node
    }
    else // If list is empty, new_node becomes the head
    {
        *head = new_node;
    }
    return (new_node);
}

/**
 * delete_node_at_index - Deletes a node at a given index from the list.
 * @head: Address of pointer to the first node.
 * @index: Index of the node to delete (0-based).
 * Return: 1 on success, 0 on failure (e.g., index out of bounds).
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head) // Empty list or invalid head pointer
		return (0);

	if (!index) // Deleting the head node
	{
		node = *head;
		*head = (*head)->next; // New head is the next node
		free(node->str);      // Free string allocated for the node
		free(node);           // Free the node structure itself
		return (1);
	}
	node = *head;
	while (node) // Traverse the list to find the node at the specified index
	{
		if (i == index) // Found the node to delete
		{
			prev_node->next = node->next; // Bypass the node to delete
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node; // Keep track of the previous node
		node = node->next;
	}
	return (0); // Index not found
}

/**
 * free_list - Frees all nodes of a linked list.
 * @head_ptr: Address of pointer to the head node.
 * Return: void.
 */
void free_list(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr) // Empty list or invalid head pointer
		return;
	head = *head_ptr;
	node = head;
	while (node) // Traverse and free each node
	{
		next_node = node->next; // Save pointer to next node before freeing current
		free(node->str);        // Free string in the node
		free(node);             // Free the node itself
		node = next_node;       // Move to the next node
	}
	*head_ptr = NULL; // Set head pointer to NULL after freeing all nodes
}
