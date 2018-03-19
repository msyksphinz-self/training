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

public final class MOVN_S extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "MOVN_S",
          MOVN_S.class,
          null
          );
      addArgument("fd", F.INFO);
      addArgument("fs", F.INFO);
      addArgument("rt", R.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final F fd = (F) getArgument("fd", args);
      final F fs = (F) getArgument("fs", args);
      final R rt = (R) getArgument("rt", args);

      return new MOVN_S(fd, fs, rt);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final F fd;
  public final F fs;
  public final R rt;

  public MOVN_S(final F fd, final F fs, final R rt) {
    assert F.INFO.isSupported(fd);
    assert F.INFO.isSupported(fs);
    assert R.INFO.isSupported(rt);

    this.fd = fd;
    this.fs = fs;
    this.rt = rt;

    addArgument("fd", fd);
    addArgument("fs", fs);
    addArgument("rt", rt);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("MOVN_S %s, %s, %s", fd.text(vars__), fs.text(vars__), rt.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("01000110000%s%s%s010011", rt.image(vars__), fs.image(vars__), fd.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    if (!rt.access(pe__, vars__).load().equals(Data.valueOf(WORD, 0x0))) {
      fd.access(pe__, vars__).store(fs.access(pe__, vars__).load());
    }
  }
}