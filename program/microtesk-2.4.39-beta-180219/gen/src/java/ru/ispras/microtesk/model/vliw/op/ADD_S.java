/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.vliw.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.vliw.PE;
import ru.ispras.microtesk.model.vliw.TempVars;
import ru.ispras.microtesk.model.vliw.mode.*;

import static ru.ispras.microtesk.model.vliw.TypeDefs.*;

public final class ADD_S extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "ADD_S",
          ADD_S.class,
          null
          );
      addArgument("fd", F.INFO);
      addArgument("fs", F.INFO);
      addArgument("ft", F.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final F fd = (F) getArgument("fd", args);
      final F fs = (F) getArgument("fs", args);
      final F ft = (F) getArgument("ft", args);

      return new ADD_S(fd, fs, ft);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final F fd;
  public final F fs;
  public final F ft;

  public ADD_S(final F fd, final F fs, final F ft) {
    assert F.INFO.isSupported(fd);
    assert F.INFO.isSupported(fs);
    assert F.INFO.isSupported(ft);

    this.fd = fd;
    this.fs = fs;
    this.ft = ft;

    addArgument("fd", fd);
    addArgument("fs", fs);
    addArgument("ft", ft);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("ADD_S %s, %s, %s", fd.text(vars__), fs.text(vars__), ft.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("01000110000%s%s%s000000", ft.image(vars__), fs.image(vars__), fd.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    fd.access(pe__, vars__).store(fs.access(pe__, vars__).load().add(ft.access(pe__, vars__).load()));
  }
}