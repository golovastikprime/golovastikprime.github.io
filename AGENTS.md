# Repository guidance

## Layout and build model

- `index.html` is the theory-library homepage. `styles.css` is the shared visual system; `app.js` powers only the homepage search and semester filters.
- `algorithmic-problem-solving/` is the static HTML learning area:
  - `index.html` links its four sections.
  - `patterns/index.html` and `patterns/_template/index.html` hold the pattern index and blank pattern template.
  - `problem-journal/index.html` uses `problem-journal/Sem_X/Contest_N/Task_K/index.html`; the `Sem_X/` and `Contest_N/` directories also have indexes. A task directory may include an accepted-code attachment such as `F.cpp` when the external solution repository is not publicly reachable.
  - `mistake-log/index.html` and `mistake-log/_template/index.html` hold the mistake index and blank entry template.
  - `review-queue/index.html` and `review-queue/_template/index.html` hold the review index and blank item template.
- `conspecta/` preserves the existing theory archive. Published notes are PDFs. Editable Typst sources currently live in `conspecta/2/calculus-2/` and `conspecta/2/chemistry-2/`.
- `scripts/serve-site.mjs` serves the repository locally; `scripts/check-site.mjs` checks static-site links and structure.
- `.github/workflows/compile_typst_to_pdf.yml` compiles Typst entrypoints and then publishes the repository root to GitHub Pages. There is no general bundling step and no production JavaScript dependency.

## Commands

Run from the repository root:

```text
node scripts/serve-site.mjs
node scripts/check-site.mjs
node --check app.js
node --check scripts/check-site.mjs
node --check scripts/serve-site.mjs
git diff --check
typst compile conspecta/2/calculus-2/calculus-2.typ conspecta/2/pdfs/calculus-2.pdf
typst compile conspecta/2/chemistry-2/chemistry-2.typ conspecta/2/pdfs/chemistry-2.pdf
```

The server command is for interactive preview. Run the two Typst commands when validating Typst-source changes; they may download Typst preview packages on first use.

## Content conventions

- Keep pages dependency-free, UTF-8 static HTML. Reuse `styles.css`, the established typography, and the existing header/footer structure; do not introduce a framework or production dependency without approval.
- Use root-absolute internal links such as `/algorithmic-problem-solving/patterns/` so nested journal pages resolve consistently. External links opened in a new tab must include `rel="noreferrer"`.
- Journal pages follow `algorithmic-problem-solving/problem-journal/Sem_X/Contest_N/Task_K/index.html`. Links to the separate solution repository follow its matching `Sem_X/Contest_N/Task_K` path. The solution repository base URL has not been supplied: leave a clearly marked placeholder instead of guessing it.
- Keep the separate solution repository canonical. When the user explicitly requests a local accepted-code attachment, store a source-equivalent snapshot beside the task page and link it with a root-absolute path; do not turn the study site into the primary solution archive.
- A task page should cross-link the relevant pattern pages, existing theory under `/conspecta/`, external solution code or an explicitly requested local attachment, applicable mistake entries, and review items. Use `<time datetime="YYYY-MM-DD">` for machine-readable dates.
- Keep `_template` pages and example placeholders visibly labeled as templates. Do not present placeholders as completed study records.

Never fabricate the user's task solutions, postmortems, recurring mistakes, root causes, self-check rules, or learning reflections. Templates remain blank until the user supplies personal content.

## Verification and repository safety

- Run the site checker, all three JavaScript syntax checks, and `git diff --check` after relevant edits.
- Preview with the local server at desktop, tablet, and mobile widths. Confirm the homepage, `/conspecta/`, `/conspecta/2/algosi/`, the algorithmic landing page, all four section indexes, and affected nested pages remain reachable.
- If Typst changes, compile both current entrypoints with the commands above and check their PDF links.
- Preserve unrelated working-tree changes. Do not commit or push unless the user explicitly requests it.
