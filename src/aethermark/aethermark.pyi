class Node:
    """Represents a node in a tree structure."""

    # ────────── Constructors ────────── #
    def __init__(self, type: str, sourcepos: list[list[int]] = None) -> None:
        """
        Initializes a Node object.

        Args:
            type (str): The type of the node.
            sourcepos (list[list[int]], optional): The source position. Defaults to None.
        """
        ...
    # ────────── Methods ────────── #
    def append_child(self, child: 'Node') -> None:
        """Appends a child node."""
        ...

    def prepend_child(self, child: 'Node') -> None:
        """Prepends a child node."""
        ...

    def unlink(self) -> None:
        """Removes the node from its parent."""
        ...

    def insert_after(self, sibling: 'Node') -> None:
        """Inserts the node after a sibling."""
        ...

    def insert_before(self, sibling: 'Node') -> None:
        """Inserts the node before a sibling."""
        ...

    def walker(self) -> 'NodeWalker':
        """Creates a NodeWalker starting at this node."""
        ...

    # ────────── Properties ────────── #
    @property
    def type(self) -> str:
        """Gets or sets the type of the node."""
        ...

    @type.setter
    def type(self, value: str) -> None:
        ...

    @property
    def sourcepos(self) -> list[list[int]]:
        """Gets or sets the source position of the node."""
        ...

    @sourcepos.setter
    def sourcepos(self, value: list[list[int]]) -> None:
        ...

    @property
    def literal(self) -> str:
        """Gets or sets the literal value of the node."""
        ...

    @literal.setter
    def literal(self, value: str) -> None:
        ...

    @property
    def destination(self) -> str:
        """Gets or sets the destination of the node."""
        ...

    @destination.setter
    def destination(self, value: str) -> None:
        ...

    @property
    def title(self) -> str:
        """Gets or sets the title of the node."""
        ...

    @title.setter
    def title(self, value: str) -> None:
        ...

    @property
    def info(self) -> str:
        """Gets or sets additional info for the node."""
        ...

    @info.setter
    def info(self, value: str) -> None:
        ...

    @property
    def level(self) -> int:
        """Gets or sets the level of the node."""
        ...

    @level.setter
    def level(self, value: int) -> None:
        ...

    # ────────── Readonly Properties ────────── #
    @property
    def first_child(self) -> 'Node':
        """Gets the first child of the node (read-only)."""
        ...

    @property
    def last_child(self) -> 'Node':
        """Gets the last child of the node (read-only)."""
        ...

    @property
    def next(self) -> 'Node':
        """Gets the next sibling of the node (read-only)."""
        ...

    @property
    def prev(self) -> 'Node':
        """Gets the previous sibling of the node (read-only)."""
        ...

    @property
    def parent(self) -> 'Node':
        """Gets the parent of the node (read-only)."""
        ...

    @property
    def container(self) -> bool:
        """Checks if the node is a container (read-only)."""
        ...

class NodeWalker:
    """A tree walker for traversing Node structures."""

    # ────────── Constructors ────────── #
    def __init__(self, root: Node) -> None:
        """
        Initializes a NodeWalker starting at the given root node.

        Args:
            root (Node): The root node to start traversal from.
        """
        ...

    # ────────── Methods ────────── #
    def next(self) -> tuple[bool, 'Node']:
        """
        Advances to the next node in the traversal.

        Returns:
            Tuple[bool, Node]:
                - A boolean indicating whether we are entering the node.
                - The current node, or None if traversal is finished.
        """
        ...

    def resume_at(self, node: Node, entering: bool) -> None:
        """
        Resumes traversal at the given node.

        Args:
            node (Node): The node to resume traversal at.
            entering (bool): Whether to start in the entering state.
        """
        ...
