import { existsSync, readdirSync, readFileSync, statSync } from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(fileURLToPath(new URL('..', import.meta.url)));
const SKIPPED_DIRECTORIES = new Set(['.git', 'node_modules']);
const SITE_HOSTS = new Set(['golovastikprime.github.io']);

const errors = [];
let checkedReferences = 0;

function toPosix(relativePath) {
  return relativePath.split(path.sep).join('/');
}

function displayPath(filePath) {
  return toPosix(path.relative(ROOT, filePath));
}

function report(filePath, line, message) {
  errors.push({ filePath: displayPath(filePath), line, message });
}

function lineNumber(source, offset) {
  let line = 1;
  for (let index = 0; index < offset; index += 1) {
    if (source.charCodeAt(index) === 10) line += 1;
  }
  return line;
}

function walk(directory, predicate, results = []) {
  for (const entry of readdirSync(directory, { withFileTypes: true })) {
    if (entry.isDirectory() && SKIPPED_DIRECTORIES.has(entry.name)) continue;

    const entryPath = path.join(directory, entry.name);
    if (entry.isDirectory()) {
      walk(entryPath, predicate, results);
    } else if (entry.isFile() && predicate(entryPath)) {
      results.push(entryPath);
    }
  }

  return results;
}

function extractTags(source) {
  const tags = [];
  let cursor = 0;

  while (cursor < source.length) {
    const start = source.indexOf('<', cursor);
    if (start === -1) break;

    if (source.startsWith('<!--', start)) {
      const commentEnd = source.indexOf('-->', start + 4);
      cursor = commentEnd === -1 ? source.length : commentEnd + 3;
      continue;
    }

    let quote = null;
    let end = start + 1;
    for (; end < source.length; end += 1) {
      const character = source[end];
      if (quote) {
        if (character === quote) quote = null;
      } else if (character === '"' || character === "'") {
        quote = character;
      } else if (character === '>') {
        break;
      }
    }

    if (end >= source.length) break;

    const raw = source.slice(start, end + 1);
    const nameMatch = raw.match(/^<\s*([a-zA-Z][\w:-]*)\b/);
    if (nameMatch) {
      const attributes = new Map();
      const attributePattern = /\b([^\s"'<>\/=]+)\s*=\s*(?:"([^"]*)"|'([^']*)'|([^\s"'=<>`]+))/g;
      let match;

      while ((match = attributePattern.exec(raw)) !== null) {
        const name = match[1].toLowerCase();
        const value = match[2] ?? match[3] ?? match[4] ?? '';
        attributes.set(name, {
          value,
          offset: start + match.index,
        });
      }

      tags.push({
        name: nameMatch[1].toLowerCase(),
        attributes,
        offset: start,
      });
    }

    cursor = end + 1;
  }

  return tags;
}

function generatedTypstPdfTargets() {
  const targets = new Set();
  const typstFiles = walk(
    path.join(ROOT, 'conspecta'),
    (filePath) => path.extname(filePath).toLowerCase() === '.typ',
  );

  for (const typstFile of typstFiles) {
    const basename = path.basename(typstFile);
    if (
      basename === 'title.typ'
      || basename === 'preambule.typ'
      || /^lecture-[0-9]+\.typ$/.test(basename)
    ) {
      continue;
    }

    const relativeParts = path.relative(ROOT, typstFile).split(path.sep);
    if (relativeParts[0] !== 'conspecta' || !relativeParts[1]) continue;

    const output = path.resolve(
      ROOT,
      'conspecta',
      relativeParts[1],
      'pdfs',
      `${path.basename(basename, '.typ')}.pdf`,
    );
    targets.add(output);
  }

  return targets;
}

function pathIsInsideRoot(filePath) {
  const relative = path.relative(ROOT, filePath);
  return relative === '' || (!relative.startsWith(`..${path.sep}`) && relative !== '..' && !path.isAbsolute(relative));
}

function resolveExistingTarget(candidate) {
  if (existsSync(candidate)) {
    const stats = statSync(candidate);
    if (stats.isFile()) return candidate;
    if (stats.isDirectory()) {
      const indexFile = path.join(candidate, 'index.html');
      if (existsSync(indexFile) && statSync(indexFile).isFile()) return indexFile;
    }
  }

  if (!path.extname(candidate)) {
    const htmlFile = `${candidate}.html`;
    if (existsSync(htmlFile) && statSync(htmlFile).isFile()) return htmlFile;
  }

  return null;
}

function decodeUrlPart(value) {
  try {
    return decodeURIComponent(value);
  } catch {
    return null;
  }
}

function parseLocalReference(rawValue) {
  const value = rawValue.trim();
  if (!value) return { error: 'empty URL' };
  if (value.startsWith('//')) return { external: true };

  if (/^[a-zA-Z][a-zA-Z\d+.-]*:/.test(value)) {
    let url;
    try {
      url = new URL(value);
    } catch {
      return { error: `malformed URL \"${value}\"` };
    }

    if ((url.protocol === 'http:' || url.protocol === 'https:') && SITE_HOSTS.has(url.hostname)) {
      return { pathname: url.pathname, fragment: url.hash.slice(1) };
    }

    return { external: true };
  }

  const hashIndex = value.indexOf('#');
  const beforeHash = hashIndex === -1 ? value : value.slice(0, hashIndex);
  const fragment = hashIndex === -1 ? '' : value.slice(hashIndex + 1);
  const queryIndex = beforeHash.indexOf('?');
  const pathname = queryIndex === -1 ? beforeHash : beforeHash.slice(0, queryIndex);
  return { pathname, fragment };
}

function validateMetadata(htmlFile, source, tags) {
  if (!/<!doctype\s+html\s*>/i.test(source)) {
    report(htmlFile, 1, 'missing <!doctype html>');
  }

  const htmlTag = tags.find((tag) => tag.name === 'html');
  const language = htmlTag?.attributes.get('lang')?.value.trim();
  if (!language) {
    report(htmlFile, htmlTag ? lineNumber(source, htmlTag.offset) : 1, 'missing non-empty <html lang="...">');
  }

  const charsetTag = tags.find(
    (tag) => tag.name === 'meta' && tag.attributes.get('charset')?.value.toLowerCase() === 'utf-8',
  );
  if (!charsetTag) report(htmlFile, 1, 'missing <meta charset="utf-8">');

  const viewportTag = tags.find(
    (tag) => tag.name === 'meta' && tag.attributes.get('name')?.value.toLowerCase() === 'viewport',
  );
  if (!viewportTag?.attributes.get('content')?.value.trim()) {
    report(htmlFile, 1, 'missing viewport metadata');
  }

  const title = source.match(/<title\b[^>]*>([\s\S]*?)<\/title\s*>/i)?.[1]
    .replace(/<[^>]*>/g, '')
    .trim();
  if (!title) report(htmlFile, 1, 'missing non-empty <title>');
}

const htmlFiles = walk(ROOT, (filePath) => path.extname(filePath).toLowerCase() === '.html');
const generatedPdfTargets = generatedTypstPdfTargets();
const documentIds = new Map();
const references = [];

for (const htmlFile of htmlFiles) {
  const source = readFileSync(htmlFile, 'utf8');
  const tags = extractTags(source);
  const ids = new Map();

  validateMetadata(htmlFile, source, tags);

  for (const tag of tags) {
    const idAttribute = tag.attributes.get('id');
    if (idAttribute) {
      const id = idAttribute.value;
      const line = lineNumber(source, idAttribute.offset);
      if (!id) {
        report(htmlFile, line, 'id attribute must not be empty');
      } else if (ids.has(id)) {
        report(htmlFile, line, `duplicate id \"${id}\" (first declared on line ${ids.get(id)})`);
      } else {
        ids.set(id, line);
      }
    }

    for (const attributeName of ['href', 'src']) {
      const attribute = tag.attributes.get(attributeName);
      if (!attribute) continue;
      references.push({
        htmlFile,
        line: lineNumber(source, attribute.offset),
        attributeName,
        value: attribute.value,
      });
    }
  }

  documentIds.set(htmlFile, ids);
}

for (const reference of references) {
  const parsed = parseLocalReference(reference.value);
  if (parsed.external) continue;
  if (parsed.error) {
    report(reference.htmlFile, reference.line, `${reference.attributeName}: ${parsed.error}`);
    continue;
  }

  checkedReferences += 1;
  const decodedPathname = decodeUrlPart(parsed.pathname);
  if (decodedPathname === null) {
    report(reference.htmlFile, reference.line, `${reference.attributeName}: malformed URL encoding in \"${reference.value}\"`);
    continue;
  }

  const candidate = decodedPathname === ''
    ? reference.htmlFile
    : decodedPathname.startsWith('/')
      ? path.resolve(ROOT, `.${decodedPathname}`)
      : path.resolve(path.dirname(reference.htmlFile), decodedPathname);

  if (!pathIsInsideRoot(candidate)) {
    report(reference.htmlFile, reference.line, `${reference.attributeName}: target escapes the site root: \"${reference.value}\"`);
    continue;
  }

  const target = resolveExistingTarget(candidate);
  const isGeneratedPdf = generatedPdfTargets.has(candidate);
  if (!target && !isGeneratedPdf) {
    report(reference.htmlFile, reference.line, `${reference.attributeName}: missing internal target \"${reference.value}\"`);
    continue;
  }

  if (!parsed.fragment || isGeneratedPdf) continue;

  const fragment = decodeUrlPart(parsed.fragment);
  if (fragment === null) {
    report(reference.htmlFile, reference.line, `${reference.attributeName}: malformed fragment encoding in \"${reference.value}\"`);
    continue;
  }

  if (target && path.extname(target).toLowerCase() === '.html') {
    const ids = documentIds.get(target);
    if (!ids?.has(fragment)) {
      report(reference.htmlFile, reference.line, `${reference.attributeName}: missing fragment \"#${fragment}\" in ${displayPath(target)}`);
    }
  }
}

errors.sort((left, right) => (
  left.filePath.localeCompare(right.filePath)
  || left.line - right.line
  || left.message.localeCompare(right.message)
));

if (errors.length > 0) {
  console.error(`Site check failed with ${errors.length} error${errors.length === 1 ? '' : 's'}:`);
  for (const error of errors) {
    console.error(`- ${error.filePath}:${error.line}: ${error.message}`);
  }
  process.exitCode = 1;
} else {
  console.log(
    `Site check passed: ${htmlFiles.length} HTML files, ${checkedReferences} internal references, `
    + `${generatedPdfTargets.size} workflow-generated PDF targets recognized.`,
  );
}
