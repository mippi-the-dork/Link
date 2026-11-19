# Maintaining Link Documentation

The Link documentation website lives in `/docs` and uses the established Mippi documentation design language.

## Documentation responsibilities

Every feature has a paired **Design** page (user-facing behavior, practical steps, expected results and troubleshooting) and **Technical** page (native editor implementation, source references, state/eligibility and limitations). Keep the content distinct; do not paste the README into both.

## Source of truth

The current Link `Source/Link/Private/LinkModule.cpp`, `Source/Link/Public/LinkModule.h`, plugin descriptor, and README establish what can be claimed as shipped. Document unverified in-editor observations as needing QA, not as passed.

## Adding features

1. Inspect the actual code and README.
2. Update both paired pages for the affected topic; add a new topic only when there is a genuinely distinct workflow.
3. Maintain the exact table row format in the five category indexes and the topic count on the home page.
4. Update [Media Inventory](MEDIA_INVENTORY.md) and replace placeholders with authentic captures or diagrams.
5. Verify search, mobile rendering, internal links, Design/Technical switch, and screenshot captions.
6. Update the [Roadmap](ROADMAP.md) only for author-approved plans or confirmed releases.

## Media captions

Each placeholder contains a reader-facing caption followed by a visually distinct **Capture instructions · Image/GIF** line. These are deliberately visible during documentation production. Every media slot has a stable ID, precise capture requirements and a future asset filename in `docs/assets/media-slots.json`. Do not invent screenshots of Unreal; capture the real plugin interface.

## Publishing

Choose GitHub Pages **Deploy from a branch → main → /docs** in repository settings. The intended URL is `https://mippi-the-dork.github.io/Link/`. This deployment must be checked separately from the merge.

## Verification

Maintain [Verification Matrix](VERIFICATION_MATRIX.md) with evidence; no source review alone establishes a passed editor/browser test.
