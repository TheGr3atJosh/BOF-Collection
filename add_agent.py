#!/usr/bin/env python3

import argparse
import re
import sys
from pathlib import Path

# Supported AXScript calls that contain an agents array.
CALLS = [
    "ax.register_commands_group",
    "menu.add_session_access",
    "menu.add_processbrowser",
]

# Regex explanation:
#
# Match calls like:
#   ax.register_commands_group(group, ["beacon"], ...)
#   menu.add_session_access(action, ["beacon"], ...)
#
# Captures:
#   prefix -> function call + first argument + comma
#   agents -> ["beacon", "gopher"]
CALL_RE = re.compile(
    r'(?P<prefix>(?:'
    + "|".join(re.escape(call) for call in CALLS)
    + r')\s*\([^,\n]+,\s*)'
    r'(?P<agents>\[[^\]]*\])'
)


def parse_agents(array_text: str) -> list[str]:
    """
    Extract:
        ["beacon", "gopher"]

    Into:
        ["beacon", "gopher"]
    """
    return re.findall(r'"([^"]+)"', array_text)


def format_agents(agents: list[str]) -> str:
    """
    Convert Python list into AXScript array syntax.
    """
    return "[" + ", ".join(f'"{agent}"' for agent in agents) + "]"


def update_content(content: str, agent_name: str, delete_mode: bool) -> tuple[str, bool]:
    """
    Add or remove agent_name in supported AXScript calls.
    """
    changed = False

    def replace(match: re.Match) -> str:
        nonlocal changed

        agents = parse_agents(match.group("agents"))

        if delete_mode:
            if agent_name not in agents:
                return match.group(0)

            agents = [a for a in agents if a != agent_name]
            changed = True

        else:
            if agent_name in agents:
                return match.group(0)

            agents.append(agent_name)
            changed = True

        return match.group("prefix") + format_agents(agents)

    return CALL_RE.sub(replace, content), changed


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Add or remove custom agents from Extension-Kit AXScript files."
    )

    parser.add_argument(
        "agent_name",
        help="Agent name (example: CrystalAgent)"
    )

    parser.add_argument(
        "--delete",
        action="store_true",
        help="Remove the agent instead of adding it"
    )

    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Show files that would be modified without writing changes"
    )

    args = parser.parse_args()

    if '"' in args.agent_name or '\n' in args.agent_name:
        print("Error: agent_name must not contain double quotes or newlines", file=sys.stderr)
        return 1

    root = Path(__file__).resolve().parent
    updated = 0

    for path in sorted(root.rglob("*.axs")):
        content = path.read_text(encoding="utf-8")

        new_content, changed = update_content(
            content,
            args.agent_name,
            args.delete
        )

        if not changed:
            continue

        updated += 1
        rel = path.relative_to(root)

        if args.dry_run:
            print(f"[DRY-RUN] Would update: {rel}")
        else:
            path.write_text(new_content, encoding="utf-8")
            print(f"[+] Updated: {rel}")

    action = "removed from" if args.delete else "added to"

    print(f"--- Complete: {args.agent_name} {action} {updated} file(s) ---")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())