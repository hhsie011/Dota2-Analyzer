import Nav from "./Nav";

export default function Container({ children }) {
  return (
    <>
      <Nav />
      {children}
    </>
  );
}
