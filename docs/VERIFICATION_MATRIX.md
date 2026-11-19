# Link v1.0.0 Verification Matrix

**Target:** UE 5.8.3, Win64, Link v1.0.0. Other compatible UE 5.8.x setups should record the exact build.  
**Status:** All editor and browser scenarios below are **Not run**, unless explicitly marked source-reviewed. An inspected C++ path is not a passing editor test.

## Capture environment when executing

Record: exact Unreal build, Link commit or release ZIP, viewport layout, actor/cinematic locks, installed plugins, tester, date, screenshots or GIFs, and Output Log/test report links.

## Editor behavior

| ID | Scenario | Expected observation | Status |
| --- | --- | --- | --- |
| LINK-01 | Install into fresh UE 5.8.3 Editor project, open Level Editor viewport | Link toolbar controls appear beside camera actions | Not run |
| LINK-02 | Two Perspective viewports, first Source, second Target | One Source and one Target indicated in their own toolbars | Not run |
| LINK-03 | Assign another Source or make Source a Target | Old Source cleared; roles never overlap | Not run |
| LINK-04 | Add several Perspective/Orthographic Targets | All valid Targets are tracked independently | Not run |
| LINK-05 | Relative X/Y/Z: move Source by known delta | Target translation adds same world-axis delta, retaining offset | Not run |
| LINK-06 | Relative Yaw enabled: rotate Source | Perspective Target Yaw follows delta, position doesn't orbit Source | Not run |
| LINK-07 | Absolute X/Y with Z disabled | Target matches horizontal position on Source movement, retains Z | Not run |
| LINK-08 | Switch Relative ↔ Absolute without moving Source | No immediate Target jump | Not run |
| LINK-09 | Toggle an axis while source remains still | Target doesn't move until future Source change or Sync | Not run |
| LINK-10 | Sync while Link is off and mode is Relative | Enabled axes copy immediately without changing mode/enabled state | Not run |
| LINK-11 | Sync with X/Y and Yaw only | Copies exactly enabled axes on Perspective Targets | Not run |
| LINK-12 | Orthographic Top/Front Targets | Position follows, orientation and zoom remain unchanged | Not run |
| LINK-13 | Choose Orthographic Source | Source role can show, but no active propagation/Sync | Not run |
| LINK-14 | Camera-lock Source to Actor or cinematic | Continuous linking pauses; Sync unavailable | Not run |
| LINK-15 | Lock one Target; leave another unlocked | Locked Target skipped, unlocked Target still follows | Not run |
| LINK-16 | Enter PIE then exit | No PIE camera changes; no accumulated first-tick movement on exit | Not run |
| LINK-17 | Disable Link, move Source, re-enable | Missed travel not applied; new movement follows | Not run |
| LINK-18 | Settings → Clear | Link off, roles cleared, X/Y/Z on, rotation off, Relative, cameras untouched | Not run |
| LINK-19 | Restart Unreal | Link configuration returns to defaults; no level content changed | Not run |
| LINK-20 | Rebuild viewport layout with same config identity | Valid viewports reconnect without stale camera delta | Not run |
| LINK-21 | Open specialized Asset Editor viewport | Link Level Editor toolbar integration not wrongly applied | Not run |
| LINK-22 | Four active Level Editor views with sustained movement | No obvious stutter or feedback loop; multiple Targets update | Not run |

## Documentation site

| ID | Scenario | Expected observation | Status |
| --- | --- | --- | --- |
| DOC-01 | Navigate all five categories and 18 topic pairs | 36 Design/Technical URLs and switches work | Link/source reviewed; browser Not run |
| DOC-02 | Search title, then phrase present only inside a page body | Immediate title match and later content/snippet match | Source reviewed; browser Not run |
| DOC-03 | Test desktop, 375px and 320px viewport widths | Article, side navigation and image figures stay usable | Not run |
| DOC-04 | View Link hero, header SVG and all 28 placeholders | Assets render with legible captions and capture instructions | Asset references checked; browser Not run |
| DOC-05 | Use keyboard search (/ and Ctrl/Cmd+K), arrows, Enter and Escape | Search accessible and functioning | Not run |
| DOC-06 | Navigate direct ?page= links, anchors and browser Back | Correct current page, headings and URL | Not run |
| DOC-07 | Test failed/slow Markdown fetch | Friendly error/result fallback; no endless loading | Not run |
| DOC-08 | Test GitHub Pages live address after merge | Site is deployed from main /docs, not merely committed | Not run |

## Result discipline

Only mark a case **Passed** after executing it, recording exact version, observed result, and evidence. Use **Failed**, **Blocked**, or **Not applicable** where appropriate. Do not conflate source reading, static Markdown link checking or JavaScript syntax parsing with live Unreal/website validation.

[Documentation Audit](DOCUMENTATION_AUDIT.md) · [Roadmap](ROADMAP.md)
