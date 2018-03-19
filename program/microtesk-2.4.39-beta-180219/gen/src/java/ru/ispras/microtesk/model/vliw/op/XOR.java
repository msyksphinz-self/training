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

public final class XOR extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "XOR",
          XOR.class,
          null
          );
      addArgument("rd", R.INFO);
      addArgument("rs", R.INFO);
      addArgument("rt", R.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R rd = (R) getArgument("rd", args);
      final R rs = (R) getArgument("rs", args);
      final R rt = (R) getArgument("rt", args);

      return new XOR(rd, rs, rt);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R rd;
  public final R rs;
  public final R rt;

  public XOR(final R rd, final R rs, final R rt) {
    assert R.INFO.isSupported(rd);
    assert R.INFO.isSupported(rs);
    assert R.INFO.isSupported(rt);

    this.rd = rd;
    this.rs = rs;
    this.rt = rt;

    addArgument("rd", rd);
    addArgument("rs", rs);
    addArgument("rt", rt);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("XOR %s, %s, %s", rd.text(vars__), rs.text(vars__), rt.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("000000%s%s%s00000100110", rs.image(vars__), rt.image(vars__), rd.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    rd.access(pe__, vars__).store(rs.access(pe__, vars__).load().xor(rt.access(pe__, vars__).load()));
  }
}