# Repository guidance

## Layout and build model

- `index.html` is the theory-library homepage. `styles.css` is the shared visual system; `app.js` powers only the homepage search and semester filters.
- `algorithmic-problem-solving/index.html` is the public semester index. Its visible hierarchy is semester → contest → task and mirrors the compact list layout used by lecture/practice pages.
- `algorithmic-problem-solving/problem-journal/Sem_X/Contest_N/Task_K/index.html` is the canonical task-note layout; semester and contest directories have their own indexes. `problem-journal/index.html` is a compatibility redirect to the public semester index. A task directory may include an accepted-code attachment such as `F.cpp` when the external solution repository is not publicly reachable.
- `algorithmic-problem-solving/patterns/`, `mistake-log/`, `review-queue/`, and all `_template` or placeholder paths are internal authoring scaffolds. Keep them unlinked from the public solution hierarchy unless the user explicitly asks to publish one of those areas.
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
- Keep filesystem identifiers such as `Sem_1/Contest_4/Task_F` in paths and explicit identifier fields, but use human-readable labels such as `Семестр 1`, `Контест 4`, and `Задача F` in page titles, headings, breadcrumbs, and indexes.
- Use `.math-inline` and `.math-display` for mathematical notation in study entries. Reserve `<code>` for source code, pseudocode, filenames, and repository identifiers.
- Journal pages follow `algorithmic-problem-solving/problem-journal/Sem_X/Contest_N/Task_K/index.html`. Links to the separate solution repository follow its matching `Sem_X/Contest_N/Task_K` path. The solution repository base URL has not been supplied: leave a clearly marked placeholder instead of guessing it.
- Keep the separate solution repository canonical. When the user explicitly requests a local accepted-code attachment, store a source-equivalent snapshot beside the task page and link it with a root-absolute path; do not turn the study site into the primary solution archive.
- A task page should link existing relevant theory under `/conspecta/` and external solution code or an explicitly requested local attachment. Add pattern or mistake links only when real published entries exist. Keep unavailable links and review reminders as internal HTML comments rather than visible UI. Use `<time datetime="YYYY-MM-DD">` for machine-readable dates.
- Keep `_template` pages and example placeholders clearly labeled inside the files, but do not link them from public indexes. Never present placeholders as completed study records.

Never fabricate the user's task solutions, postmortems, recurring mistakes, root causes, self-check rules, or learning reflections. Templates remain blank until the user supplies personal content.

## Verification and repository safety

- Run the site checker, all three JavaScript syntax checks, and `git diff --check` after relevant edits.
- Preview with the local server at desktop, tablet, and mobile widths. Confirm the homepage, `/conspecta/`, `/conspecta/2/algosi/`, and every level of the public solution hierarchy remain reachable. The site checker validates the retained internal scaffold pages too.
- If Typst changes, compile both current entrypoints with the commands above and check their PDF links.
- Preserve unrelated working-tree changes. Do not commit or push unless the user explicitly requests it.
